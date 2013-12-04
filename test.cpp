#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "fileStatus.h"
#include "fileParsing.h"
#include "tbb/flow_graph.h"
#include <stdlib.h>
#include <functional>
#include "tbb/mutex.h"
#include "tbb/tbb_thread.h"
using namespace tbb::flow;
using namespace std;
static tbb::mutex mut;

//todo: put a lock on dirtyFiles
//
bool checkIfParentsHaveChanged( vector<DependencyNode*> dependencies,  map<string,bool> dirtyFiles ){
  bool unchanged = true;
  tbb::mutex::scoped_lock lock;
  lock.acquire(mut);
  for(int i = 0; i < dependencies.size(); i++){
    string parentFile = dependencies[i]->target;
    bool parentUnChanged = !dirtyFiles[parentFile];
    unchanged = unchanged && parentUnChanged;
  }
  lock.release();
  return !unchanged;
}

void buildInParallel( map<string,bool>& dirtyFiles, StringToDepNodeMap dnMap){
  //for each node in the dependency graph, make a function node
  //todo: add logic where it only finds the relevant parts of the tree to traverse (this can just be a flat list of names)
  //todo: add logic where if everything is fresh, then you don't build at all.
  graph g;

  broadcast_node<continue_msg> input(g);
  map<string,continue_node<continue_msg>* > continueNodes; 
  
  for (map<string,DependencyNode*>::iterator it = dnMap.begin(); it != dnMap.end(); it++){
    string name = it -> first;
    DependencyNode* node = dnMap[name];
    bool fileHasChangedOnDisk = dirtyFiles[name];
    
    continue_node<continue_msg> * f = new continue_node<continue_msg>( g,  [=]( const continue_msg& ){ 
	bool parentsChanged = checkIfParentsHaveChanged(node->dependencies,dirtyFiles);
	bool needToBuild = fileHasChangedOnDisk || parentsChanged;
	if(needToBuild){
	  node->doBuild();
	}

	tbb::mutex::scoped_lock lock;
	lock.acquire(mut);
	dirtyFiles[name] = needToBuild;
	lock.release();

      });
    continueNodes[node->target]= f;
  }

  map<string,continue_node<continue_msg>* >::iterator iter;
  for(iter = continueNodes.begin(); iter != continueNodes.end(); iter++){
    DependencyNode* depNode = dnMap[iter->first];
    continue_node<continue_msg>*node  = iter->second;
    //if it is a leaf node, connect it to the input node, else connect it to its parents
    if(depNode->dependencies.size() == 0){
      make_edge(input,*node);
    }
    for(int i = 0; i < depNode->dependencies.size(); i++){

      continue_node<continue_msg>* parent = continueNodes[ depNode->dependencies[i]->target ];
      make_edge(*parent,*node);
    } 
    

  }
  input.try_put( continue_msg() );

  g.wait_for_all();
	
}


int main() {

  //First, parse the Remodel file 
  string fn = "RemodelFile";
  StringToDepNodeMap dnMap;
  processRemodelFile(fn, dnMap);
  vector<string> files = getKeys(dnMap);

  //printDependencies(dnMap); //use this for debugging

  //Then find out the status of all files in the dependencies (i.e. whether they have changed on disk). 
  //here, the value of the map is true if the file has changed on disk
  map<string,bool> FileStatus;
  getFileStatuses(files,FileStatus); 

  string n = "foo.cpp";
  FileStatus[n] = true;
  //todo: make list of things that you care about (by traversing up along dependencies).
  buildInParallel(FileStatus,dnMap);

  return 0;
}



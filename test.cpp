#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "fileStatus.h"
#include "fileParsing.h"
#include "tbb/flow_graph.h"
#include <stdlib.h>
#include <functional>
using namespace tbb::flow;
using namespace std;


//todo: have DependencyNode have all the functionality of a TBBDepNode.
//todo: test this first on something simple
//todo: how do you send messages to all the things to start?
//todo: candidate solution: have one input node, set to false (i.e. nothing has changed). keep a list of all the things that are leaves (i.e. have no parents). 
//This can be trivially discovered while traversing dnMap.



void buildInParallel(map<string,bool> fs, StringToDepNodeMap dnMap){
  //for each node in the dependency graph, make a function node
  //todo: add logic where it only finds the relevant parts of the tree to traverse (this can just be a flat list of names)
  //todo: add logic where if everything is fresh, then you don't build at all.
  graph g;
  broadcast_node<bool> input(g);
  map<string,function_node<bool,bool>* > functionNodes; 
  for (map<string,DependencyNode*>::iterator it = dnMap.begin(); it != dnMap.end(); it++){
    string name = it -> first;
    bool fileHasNotChangedOnDisk = fs[name];
    DependencyNode* node = dnMap[name];
    node -> fileHasChanged = !fileHasNotChangedOnDisk;
    
    function_node<bool,bool> * f = new function_node<bool,bool>( g, unlimited, [=](bool a ) -> bool{ return node->build(a);});
    functionNodes[node->target]= f;
  }

  map<string,function_node<bool,bool>* >::iterator iter;
  for(iter = functionNodes.begin(); iter != functionNodes.end(); iter++){
    DependencyNode* depNode = dnMap[iter->first];
    if(depNode->target != iter->first){
      printf("problem\n");
    }
    function_node<bool,bool>*node  = iter->second;
    //if it is a leaf node, connect it to the input node, else connect it to its parents
    printf("making an edge between input and %s @ %p and %p\n",(*depNode).target.c_str(), &input, node);
    // make_edge(input,*node);
    if(depNode->dependencies.size() == 0)
      make_edge(input,*node);
    for(int i = 0; i < depNode->dependencies.size(); i++){
      function_node<bool,bool>* parent = functionNodes[ depNode->dependencies[i]->target];
      make_edge(*parent,*node);
    } 
    

  }

  input.try_put(false);

  g.wait_for_all();
	
}

int main() {


  //First, parse the Remodel file 
  string fn = "RemodelFile"; //todo: have this point to a remodel file
  StringToDepNodeMap dnMap;
  processRemodelFile(fn, dnMap);
  vector<string> files = getKeys(dnMap);

  //  printDependencies(dnMap); //use this for debugging

  //Then find out the status of all files in the dependencies (i.e. whether they have changed on disk). 
  //here, the value of the map is true if the file *has not* changed on disk
  map<string,bool> FileStatus;
  getFileStatuses(files,FileStatus); 

  buildInParallel(FileStatus,dnMap);

  //map<string,bool>::iterator iter;
  //for(iter = FileStatus.begin(); iter != FileStatus.end(); iter++){
  //  printf("file %s has status %d\n",iter -> first.c_str() ,  (char) (iter -> second));
  // }
 
  //now, build the dependency tree from the bottom up. execute as much as you can asynchronously in parallel

  return 0;
}




#include "parallelBuild.h"
using namespace tbb::flow;
using namespace std;
static tbb::mutex mut;



bool checkIfParentsHaveChanged( vector<DependencyNode*> dependencies,  map<string,bool>& dirtyFiles ){
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


void buildInParallel(StringToDepNodeMap dnMap,map<string,bool>& FileStatus){
  //todo: make list of things that you care about (by traversing up along dependencies).
  //todo: add logic where it only finds the relevant parts of the tree to traverse (this can just be a flat list of names)
  //todo: add logic where if everything is fresh, then you don't build at all.
  graph g;
  broadcast_node<continue_msg> input(g);
  map<string,continue_node<continue_msg>* > continueNodes; 
  for (map<string,DependencyNode*>::iterator it = dnMap.begin(); it != dnMap.end(); it++){
    string name = it -> first;
    DependencyNode* node = dnMap[name];
    bool fileHasChangedOnDisk = FileStatus[name];
    continue_node<continue_msg> * f = new continue_node<continue_msg>( g,  [&FileStatus,node,name,fileHasChangedOnDisk]( const continue_msg& ){ 
	bool parentsChanged = checkIfParentsHaveChanged(node->dependencies,FileStatus);
	bool needToBuild = fileHasChangedOnDisk || parentsChanged;
	if(needToBuild){
	  node->doBuild();
	}

	tbb::mutex::scoped_lock lock;
	lock.acquire(mut);
	FileStatus[name] = needToBuild;
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



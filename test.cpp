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

  graph g;
  broadcast_node<bool> input(g);
  map<string,function_node<bool,bool> > functionNodes; 
  for (map<string,DependencyNode*>::iterator it = dnMap.begin();
        it != dnMap.end();
       ++it){
    string name = it -> first;
    bool fileHasNotChangedOnDisk = fs[name];
    DependencyNode* node = dnMap[name];
    node -> fileHasChanged = !fileHasNotChangedOnDisk;
    
    function_node<bool,bool> f( g, unlimited, [=](bool a ) -> bool{ return node->build(a);});
    map[node->target]= f;
  }

  map<string,function_node<bool,bool> >::iterator iter;
  for(iter = functionNodes.begin(); iter != functionNodes.end(); iter++){
    function_node<bool,bool> node = iter->second;
    vector<DependencyNode*>::
    make_edge(parent,node)
  }


   
    make_edge( input, squarer );
  make_edge( input, cuber );
  make_edge( squarer, get<0>( join.input_ports() ) );
  make_edge( cuber, get<1>( join.input_ports() ) );
  make_edge( join, summer );

  for (int i = 1; i <= 10; ++i)
    input.try_put(i);
  g.wait_for_all();

	
  //now, loop over the nodes again and make an edge between it and its parents (dependencies)

  
  //now, run

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



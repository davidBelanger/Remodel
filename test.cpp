#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/config.hpp> 
#include "boost/algorithm/string/erase.hpp"
#include "boost/algorithm/string/classification.hpp"
#include "fileStatus.h"
#include "fileParsing.h"
#include "tbb/flow_graph.h"
#include <stdlib.h>

using namespace tbb::flow;
using namespace std;


//todo: have DependencyNode have all the functionality of a TBBDepNode.
//todo: test this first on something simple
//todo: how do you send messages to all the things to start?
//todo: candidate solution: have one input node, set to false (i.e. nothing has changed). keep a list of all the things that are leaves (i.e. have no parents). 
//This can be trivially discovered while traversing dnMap.
void buildInParallel(map<string,bool> fs, StringToDepNodeMap dnMap){
  //for each node in the dependency graph, make a 
  graph g;
  broadcast_node<bool> input(g);
  for (map<string,DependencyNode*>::iterator it = dnMap.begin();
        it != dnMap.end();
       ++it){
    string name = it -> first;
    bool fileHasNotChangedOnDisk = fs[name];
    DependencyNode* node = dnMap[name];
    node -> fileHasChanged = !fileHasNotChangedOnDisk;
    //[&t] (bool t) node->build(t)
    DependencyNode n = *node;
    function_node<bool,bool> f( g, unlimited, n());

  }

  //now, loop over the nodes again and make an edge between it and its par

}

int main() {


  //First, parse the Remodel file 
  string fn = "dummyfile"; //todo: have this point to a remodel file
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



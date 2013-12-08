#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include<stdexcept>

using namespace std;


class DependencyNode{
 public:
  bool fileHasChanged;
  string target;
  string compile_cmd;
  vector<DependencyNode*> dependencies;
  int operator()(int v) { return v*v; }
  int hello(int v) { return v*v; }
  void doBuild(){
    bool nonTrivialBuildCommand = compile_cmd != "";
    if(nonTrivialBuildCommand){
      printf("executing: %s\n",compile_cmd.c_str());
      int returnval = std::system(compile_cmd.c_str());
      if(returnval != 0){
	throw compile_cmd;
      }	
    }
  }  
};


typedef map<string,DependencyNode*>  StringToDepNodeMap;

void cleanupNodeMap(StringToDepNodeMap dnMap);

void processRemodelFile(std::string filename, StringToDepNodeMap& m);


void printDependencies(StringToDepNodeMap dnMap);

vector<string> getKeys(StringToDepNodeMap dnMap);

void getRelevantFiles(StringToDepNodeMap dnMap,string target, vector<string> *  relevantFiles);

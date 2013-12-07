#include <string>
#include <vector>
#include <map>
#include <cstdlib>
using namespace std;

class DependencyNode{
 public:
  bool fileHasChanged;
  string target;
  string compile_cmd;
  vector<DependencyNode*> dependencies;
  int operator()(int v) { return v*v; }
  int hello(int v) { return v*v; }
  void doBuild(void){
    bool nonTrivialBuildCommand = compile_cmd != "";
    if(nonTrivialBuildCommand){
      printf("executing: %s\n",compile_cmd.c_str());
      int returnval = std::system(compile_cmd.c_str());
      //if(returnval != 0)	
      //todo: add error handling here
    }
  }
  
};

typedef map<string,DependencyNode*>  StringToDepNodeMap;

void processRemodelFile(std::string filename, StringToDepNodeMap& m);


void printDependencies(StringToDepNodeMap dnMap);

vector<string> getKeys(StringToDepNodeMap dnMap);

void getRelevantFiles(StringToDepNodeMap dnMap,string target, vector<string> *  relevantFiles);

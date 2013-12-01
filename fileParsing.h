#include <string>
#include <vector>
#include <map>
using namespace std;

class DependencyNode{
 public: 
  bool fileHasChanged;
  string target;
  string compile_cmd;
  vector<DependencyNode*> dependencies;
  bool operator()(bool someParentHasChanged){ bool dirty  = someParentHasChanged || fileHasChanged; if(dirty) system(compile_cmd.c_str())   ;return dirty;}; //todo: add some error handling

  
  //bool build(bool  someParentHasChanged){ bool dirty  = someParentHasChanged || fileHasChanged; if(dirty) system(compile_cmd.c_str())   ;return dirty;}; //todo: add some error handling

};

typedef map<string,DependencyNode*>  StringToDepNodeMap;

void processRemodelFile(std::string filename, StringToDepNodeMap& m);


void printDependencies(StringToDepNodeMap dnMap);

vector<string> getKeys(StringToDepNodeMap dnMap);


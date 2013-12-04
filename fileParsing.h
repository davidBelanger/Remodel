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
    std::system(compile_cmd.c_str());
  }
  //todo: how do we declare this as non-static?
  const bool build(bool someParentHasChanged){ 
     bool dirty  =  someParentHasChanged || fileHasChanged; 
     printf("building %s\nparentStatus = %d. Dirty = %d\n",target.c_str(),someParentHasChanged,dirty); 

     if(dirty) 
       doBuild();//todo: add some error handling
     return dirty;
   } 



  
};

typedef map<string,DependencyNode*>  StringToDepNodeMap;

void processRemodelFile(std::string filename, StringToDepNodeMap& m);


void printDependencies(StringToDepNodeMap dnMap);

vector<string> getKeys(StringToDepNodeMap dnMap);


#include <string>
#include <vector>
#include <map>
using namespace std;

struct DependencyNode{
  string target;
  string compile_cmd;
  vector<DependencyNode*> dependencies;
};

typedef map<string,DependencyNode*>  StringToDepNodeMap;

void processRemodelFile(std::string filename, StringToDepNodeMap& m);


void printDependencies(StringToDepNodeMap dnMap);

vector<string> getKeys(StringToDepNodeMap dnMap);


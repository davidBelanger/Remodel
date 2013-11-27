#include <string>
#include <vector>
using namespace std;

struct DependencyNode{
  string target;
  string compile_cmd;
  vector<DependencyNode*> dependencies;
};

typedef map<string,DependencyNode*>  StringToDepNodeMap;

void processRemodelFile(std::string filename, StringToDepNodeMap& m);



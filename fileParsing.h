#include <string>
#include <vector>
using namespace std;
struct CompilationDependency{
  string filename;
  vector<string> dependencies;
  string compilationCommand;
};

std::vector<CompilationDependency> processRemodelFile(std::string filename);

#include <string>
#include <vector>
using namespace std;
struct compileInstruction{
  string filename;
  vector<string> dependencies;
  string compilationCommand;
};

std::vector<compileInstruction> processRemodelFile(std::string filename);

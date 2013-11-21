#include <string>
#include <vector>
struct compileInstruction{
  std::string filename;
  std::string compilationCommand;
};

std::vector<compileInstruction> processRemodelFile(std::string filename);

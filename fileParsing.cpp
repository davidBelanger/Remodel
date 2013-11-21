#include <iostream>
#include <fstream>
#include <string>
#include "fileParsing.h"
#include <boost/regex.hpp>

using namespace std;

/*
const struct GrammarRegexes{
  const boost::regex re1("ello");
};
*/


vector<compileInstruction> processRemodelFile(string filename){
  vector<compileInstruction> v;
  
  ifstream file(filename.c_str());
  string content;
  
  boost::regex re1("ello");

  boost::regex re2("(\\S*) <- (.+):\\s*\"(.*)\"");

  boost::regex re3( "(\\S*)\\s+:\\s+(\\S*)");

  while(getline(file,content)) {
    boost::match_results<std::string::const_iterator> results;
    
    if(boost::regex_search(content, results, re2)){
      string target = results[1];
      string dependencies = results[2]; //todo: further split this into a list
      string command = results[3];
      printf("got target %s and dependencies %s and command %s\n",target.c_str(),dependencies.c_str(),command.c_str());
    }
  }
    
  return v;
}






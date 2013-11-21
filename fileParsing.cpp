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
  
  while(file >> content) {
    
    boost::match_results<std::string::const_iterator> results;
    
    if(boost::regex_search(content, results, re1)){
      cout << "got " << results[0] << endl;
      //      list->push_back(results[1]);
    }
  }
    
  return v;
}






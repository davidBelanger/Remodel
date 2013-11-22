#include <iostream>
#include <fstream>
#include <string>
#include "fileParsing.h"
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>



using namespace boost;
using namespace std;

/*
const struct GrammarRegexes{
  const boost::regex re1("ello");
};
*/

vector<string> parseCommaDeliminatedString(string text){
  vector<string> fields;
  char_separator<char> sep(",");
  tokenizer<char_separator<char> > tokens(text, sep);
  for ( tokenizer<char_separator<char> >::iterator it = tokens.begin();
	it != tokens.end();
	++it)
    {
      string str = *it;
      str.erase(remove(str.begin(), str.end(), ' '), str.end());
      fields.push_back(str);
    }

  return fields;
}

vector<CompilationDependency> processRemodelFile(string filename){
  vector<CompilationDependency> v;
  
  ifstream file(filename.c_str());
  string content;
  

  boost::regex re("(.*) <- (.+):\\s*\"(.*)\"");


  while(getline(file,content)) {
    boost::match_results<std::string::const_iterator> results;
    
    if(boost::regex_search(content, results, re)){
      string command = results[3];

      vector<string> targets =  parseCommaDeliminatedString(results[1]);
      vector<string> dependencies =  parseCommaDeliminatedString(results[2]);
      
      for(int i = 0; i < targets.size(); i++){
	CompilationDependency dep;
	dep.filename = targets[i];
	dep.dependencies = dependencies;
	dep.compilationCommand = command;
	v.push_back(dep);
      }

      for(int i = 0; i < v.size(); i++){
	CompilationDependency depp = v[i];
	printf("got target %s and dependencies %s, %s and command %s\n",depp.filename.c_str(),depp.dependencies[0].c_str(),depp.dependencies[1].c_str(),depp.compilationCommand.c_str());

      }


    }
  }
    
  return v;
}






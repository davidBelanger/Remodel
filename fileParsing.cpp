#include <iostream>
#include <fstream>
#include <string>
#include "fileParsing.h"
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>


using namespace boost;
using namespace std;
/*

string joinStringVector(const vector<string> vec){
  //  char* delimm = ",";
  //    stringstream res;
  //  copy(vec.begin(), vec.end(), ostream_iterator<string>(res, ","));
  //  return res.str();
  return boost::algorithm::join(vec, ",");
}
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

void printVector(vector<string> v){
  for(int i = 0; i < v.size(); i++){
    printf("%s ",v[i].c_str());
  }
}
vector<CompilationDependency> processRemodelFile(string filename){
  vector<CompilationDependency> v;
  
  ifstream file(filename.c_str());
  string content;
  

  boost::regex getProductionRegex("(.*) <- (.+):\\s*\"(.*)\"");
  boost::regex getDefaultLineRegex("DEFAULT\\s*<-\\s*(.+)");
  

  while(getline(file,content)) {
    boost::match_results<std::string::const_iterator> results;
    if(boost::regex_search(content, results, getDefaultLineRegex)){
      CompilationDependency dep;
      dep.filename = "DEFAULT";
      vector<string> res;
      res.push_back(results[1]);
      dep.dependencies = res;
      dep.compilationCommand = "root";
      v.push_back(dep);
      
    }else if(boost::regex_search(content, results, getProductionRegex)){
      //      printf("match is %s ---- %s ---- %s\n",results[1].c_str(),results[2].c_str(),results[3].c_str());
      string command = results[3];
      vector<string> targets =  parseCommaDeliminatedString(results[1]);
      vector<string> dependencies =  parseCommaDeliminatedString(results[2]);
      
      for(int i = 0; i < targets.size(); i++){
	CompilationDependency dep;
	dep.filename = targets[i];
	cout << "dependencies = ";
	printVector(dependencies);
	cout << endl;
	dep.dependencies = dependencies;
	dep.compilationCommand = command;
	v.push_back(dep);
      }
    }

  }
  for(int i = 0; i < v.size(); i++){
    CompilationDependency depp = v[i];
    printf("got target %s and command %s\n",depp.filename.c_str(),depp.compilationCommand.c_str());
    printf("with dependencies "); printVector(depp.dependencies); cout << endl;
  }


    
  return v;
}






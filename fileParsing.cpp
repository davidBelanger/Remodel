#include "fileParsing.h"
#include <iostream>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>


using namespace boost;
using namespace std;

void getOrElseUpdate(StringToDepNodeMap& dnmap,string key, DependencyNode*&  out ){
  map<string,DependencyNode*>::iterator iter = dnmap.find(key);
  if(iter != dnmap.end()){
    out = iter->second;
  }else{
    DependencyNode* newNode = new DependencyNode;
    newNode->target = key;
    out = newNode;
    dnmap[key] = newNode;
  }
}

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
void processRemodelFile(string filename, StringToDepNodeMap& dnMap){
  ifstream file(filename.c_str());
  string content;
  
  boost::regex getProductionRegex("(.*) <- (.+):\\s*\"(.*)\"");
  boost::regex getDefaultLineRegex("DEFAULT\\s*<-\\s*(.+)");
  
  
  while(getline(file,content)) {
    boost::match_results<std::string::const_iterator> results;
    if(boost::regex_search(content, results, getDefaultLineRegex)){
      DependencyNode* dep;
      string name = results[1];
      getOrElseUpdate(dnMap,"DEFAULT",dep);
      DependencyNode* parent;
      getOrElseUpdate(dnMap,name,parent);
      dep -> dependencies.push_back(parent);
      dep -> compile_cmd = "";
      dep -> target = "DEFAULT";   
    }else if(boost::regex_search(content, results, getProductionRegex)){
      string command = results[3];
      vector<string> targets =  parseCommaDeliminatedString(results[1]);
      vector<string> dependencies =  parseCommaDeliminatedString(results[2]);
      for(int i = 0; i < targets.size(); i++){
	DependencyNode* dn;
	string target = targets[i];
	getOrElseUpdate(dnMap,target,dn);
	dn -> compile_cmd = command;
	for(int j = 0; j < dependencies.size(); j++){
	  DependencyNode* dep;
	  getOrElseUpdate(dnMap,dependencies[j],dep);
	  dn -> dependencies.push_back(dep);
	}
      }
      
    }

  }

}






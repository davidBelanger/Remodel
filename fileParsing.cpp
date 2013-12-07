#include "fileParsing.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <boost/algorithm/string.hpp>
using namespace boost::algorithm;


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
  const char delim = ',';
  const char* str = text.c_str();
    do
    {
        const char *begin = str;

        while(*str != delim && *str)
            str++;
	string st = string(begin, str);
	trim(st);
        fields.push_back(st);
    } while (0 != *str++);

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
  
 
  string r1 = "DEFAULT\\s*<-\\s*(.+)";
  string r2 = "(.*) <- (.+):\\s*\"(.+)\"";
  
  regex getDefaultLineRegex(r1.c_str());
  regex getProductionRegex(r2.c_str());

  
  
  while(getline(file,content)) {
    match_results<std::string::const_iterator> results;
    if(regex_search(content, results, getDefaultLineRegex)){
      DependencyNode* dep;
      string name = results[1];
      getOrElseUpdate(dnMap,"DEFAULT",dep);
      DependencyNode* parent;
      getOrElseUpdate(dnMap,name,parent);
      dep -> dependencies.push_back(parent);
      dep -> compile_cmd = "";
      dep -> target = "DEFAULT";   
    }else if(regex_search(content, results, getProductionRegex)){
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


void printDependencies(StringToDepNodeMap dnMap){
   map<string,DependencyNode*>::iterator iter;
  for(iter = dnMap.begin(); iter != dnMap.end(); iter++){
    printf("key = --%s--\n",iter->first.c_str());
    DependencyNode dn = *(iter->second);
    if(strcmp(iter -> first.c_str(),dn.target.c_str())){
      printf("error\n");
    }
    printf("target: %s compile cmd: %s\n",dn.target.c_str(),dn.compile_cmd.c_str());
    printf("deps:");
    for(int j = 0; j < dn.dependencies.size(); j++){
      printf(" %s", dn.dependencies[j]->target.c_str());
    }
    printf("\n\n");

  }

}

vector<string> getKeys(StringToDepNodeMap dnMap){
  vector<string> tr;
  StringToDepNodeMap::iterator iter;
  for(iter = dnMap.begin(); iter != dnMap.end(); iter++){
    tr.push_back(iter-> first);
  }
  return tr;
}


void getRelevantFiles(StringToDepNodeMap dnMap,string target,vector<string> *  relevantFiles){
  relevantFiles->push_back(target);
  DependencyNode* node = dnMap[target];
  for(int i = 0; i < node->dependencies.size(); i++){
      getRelevantFiles(dnMap,node->dependencies[i]->target,relevantFiles);
  }
}




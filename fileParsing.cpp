#include <iostream>
#include <fstream>
#include <string>
#include "fileParsing.h"
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>


using namespace boost;
using namespace std;


//keep a hashmap from strings to dependency tree nodes
//each dependency node has:
//1) a string for the system call to build it
//2) pointers to other dependency nodes
//3) when you instantiate something, call getorelseupdate on the hashmap

struct DependencyNode{
  string target;
  string compile_cmd;
  vector<DependencyNode*> dependencies;
};

typedef map<string,DependencyNode*>  StringToDepNodeMap;

void getOrElseUpdate(StringToDepNodeMap dnmap,string key, DependencyNode*  out ){
  map<string,DependencyNode*>::iterator iter = dnmap.find(key);
  if(iter != dnmap.end()){
    out = iter->second;
  }else{
    DependencyNode* newNode = (DependencyNode*) malloc(sizeof(DependencyNode));
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
vector<CompilationDependency> processRemodelFile(string filename){
  vector<CompilationDependency> v;
  
  ifstream file(filename.c_str());
  string content;
  

  boost::regex getProductionRegex("(.*) <- (.+):\\s*\"(.*)\"");
  boost::regex getDefaultLineRegex("DEFAULT\\s*<-\\s*(.+)");
  
  StringToDepNodeMap dnMap;
  
  while(getline(file,content)) {
    boost::match_results<std::string::const_iterator> results;
    //todo: push a DepNode for default
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
      printf("here");
      for(int i = 0; i < targets.size(); i++){
	CompilationDependency dep;
	dep.filename = targets[i];
	//	cout << "dependencies = ";
	//printVector(dependencies);
	//cout << endl;
	dep.dependencies = dependencies;
	dep.compilationCommand = command;
	v.push_back(dep);
      }
      
      printf("printing one");
      for(int i = 0; i < targets.size(); i++){
	DependencyNode* dn;
	string target = targets[i];
	getOrElseUpdate(dnMap,target,dn);
	printf("got dn");
	dn -> compile_cmd = command;
	for(int j = 0; j < dependencies.size(); j++){
	  DependencyNode* dep;
	  getOrElseUpdate(dnMap,dependencies[j],dep);
	  dn -> dependencies.push_back(dep);
	}
      }
      
    }

  }

  printf("about to print dependencies\n");
  map<string,DependencyNode*>::iterator iter;
  for(iter = dnMap.begin(); iter != dnMap.end(); iter++){
    DependencyNode dn = *(iter->second);
    if(strcmp(iter -> first.c_str(),dn.target.c_str())){
      printf("error\n");
    }
    printf("%s %s\n",dn.target.c_str(),dn.compile_cmd.c_str());
    printf("deps:");
    for(int j = 0; j < dn.dependencies.size(); j++){
      printf(" %s", dn.dependencies[j]->target.c_str());
    }

  }




  /*
  for(int i = 0; i < v.size(); i++){
    CompilationDependency depp = v[i];
    printf("got target %s and command %s\n",depp.filename.c_str(),depp.compilationCommand.c_str());
    printf("with dependencies "); printVector(depp.dependencies); cout << endl;
  }
  */


    
  return v;
}






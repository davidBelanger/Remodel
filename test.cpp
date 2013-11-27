#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/config.hpp> 
#include "boost/algorithm/string/erase.hpp"
#include "boost/algorithm/string/classification.hpp"
#include "fileStatus.h"
#include "fileParsing.h"
#include "tbb/task_scheduler_init.h"

using namespace std;

vector<string>* getFiles(string filename){
  ifstream file(filename.c_str());
  string content;
  vector<string>* list = new vector<string>;
  while(file >> content) {
    list->push_back(content);
  }

  return list;
}




//todo: write a recursive method for building things when their parents are builts

int main() {

  //todo: what should this return?
  string fn = "dummyfile";
  StringToDepNodeMap dnMap;
  processRemodelFile(fn, dnMap);

  //  printDependencies(dnMap); //use this for debugging
  
  //todo: change this to get the filenames from what it got from the makefile
  string filename = "ReMakeFile";
  cout << "reading files from " << filename << endl;
  vector<string> files = *getFiles(filename);
    

  map<string,bool> FileStatus;
  getFileStatuses(files,FileStatus);
  

  return 0;
}



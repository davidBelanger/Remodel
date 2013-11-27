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




int main() {


  //First, parse the Remodel file 
  string fn = "dummyfile"; //todo: have this point to a remodel file
  StringToDepNodeMap dnMap;
  processRemodelFile(fn, dnMap);
  vector<string> files = getKeys(dnMap);

  //  printDependencies(dnMap); //use this for debugging

  //Then find out the status of all files in the dependencies (i.e. whether they have changed on disk). 
  map<string,bool> FileStatus;
  getFileStatuses(files,FileStatus);
  //map<string,bool>::iterator iter;
  //for(iter = FileStatus.begin(); iter != FileStatus.end(); iter++){
  //  printf("file %s has status %d\n",iter -> first.c_str() ,  (char) (iter -> second));
  // }
 
  //now, build the dependency tree from the bottom up. execute as much as you can asynchronously in parallel

  return 0;
}



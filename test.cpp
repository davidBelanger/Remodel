#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "fileStatus.h"
//#include "fileParsing.h"
#include "parallelBuild.h"

int main() {

  //First, parse the Remodel file 
  string fn = "RemodelFile";
  StringToDepNodeMap dnMap;
  processRemodelFile(fn, dnMap);
  vector<string> files = getKeys(dnMap);

  //printDependencies(dnMap); //use this for debugging

  //Then find out the status of all files in the dependencies (i.e. whether they have changed on disk). 
  //here, the value of the map is true if the file has changed on disk
  map<string,bool>  FileStatus;
  
   getFileStatuses(files,FileStatus); 

   //now run
   buildInParallel(dnMap,FileStatus);

  return 0;
}



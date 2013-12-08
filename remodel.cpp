#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "fileStatus.h"
//#include "fileParsing.h"
#include "parallelBuild.h"




int main(int argc, char** argv) {

  if(argc > 2){
    printf("usage: remake <target>\n(if <target> is empty, it will make the target specified by DEFAULT <- <target> in the RemodelFile file\n");
    exit(1);
  }

  string target = "DEFAULT";
  if(argc == 2){
    target = argv[1];
  }
  
  
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

   map<string,bool>::iterator iter = FileStatus.find(target);
   if( iter == FileStatus.end() ){
     printf("error: can't find information for building %s in RemodelFile\n",target.c_str());
     cleanupNodeMap(dnMap);
     exit(1);
   }

   //traverse up from the target in the dependency tree and find out what intermediate targets we care about. Only build these. 
   vector<string> filesToBuild;
   getRelevantFiles(dnMap,target,&filesToBuild);

 
   buildInParallel(filesToBuild,dnMap,FileStatus);

   cleanupNodeMap(dnMap);

   
  return 0;
}



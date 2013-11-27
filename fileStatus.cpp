#include "fileStatus.h"


char* computeMD5(const char* filename ){
  FILE *in = fopen (filename, "rb");
  if(in == NULL){
    printf("error: file %s doesn't exist, but trying to compute md5 of it\n", filename);
  }
  MD5_CTX mdContext;
  int bytes;
  unsigned char digest[16];
  unsigned char data[1024];
  MD5_Init (&mdContext);
  while ((bytes = fread (data, 1, 1024, in)) != 0)
    MD5_Update (&mdContext, data, bytes);
  //  unsigned char c[MD5_DIGEST_LENGTH];
  char *c = (char*)malloc(33);
  MD5_Final (digest,&mdContext);
    int n;
    for (n = 0; n < MD5_DIGEST_LENGTH; ++n) {
      snprintf(&(c[n*2]), 16*2, "%02x", (unsigned int)digest[n]);
    }

    return c;
    
}

void loadMD5Values(string filename, map<string,string>& md5values){
  ifstream file(filename.c_str());
  string content;
  boost::regex getProductionRegex("(.*) <- (.+):\\s*\"(.*)\"");
  while(getline(file,content)) {
    std::vector<std::string> words;
    boost::split(words, content, boost::is_space());
    md5values[words[0]] = words[1];
  }
  file.close();
}

bool mapContains(map<string,string> m, string key){
   map<string,string>::iterator iter = m.find(key);
   bool returnval = ( iter != m.end() );    
   return returnval;
}

bool fileExists(string fn){
  ifstream is(fn.c_str());
  bool tr = is;
  return tr;
}

void getFileStatuses(vector<string> files,map<string,bool>& FileStatus){
    vector<string>::const_iterator cii;
  //check if the md5 values are stored on disk already. if not, you assume that everything needs to be built from scratch
  string md5file = "./remodel/.md5-map";
  bool md5fileExists = fileExists(md5file);

  map<string,string> md5values;
  
  if(md5fileExists){
    loadMD5Values(md5file,md5values);
  }


  //now loop over the files that we care about and check whether they are up-to-date or not
  ofstream md5fileNew;
  string temp_md5file = "./remodel/.md5-map.tmp";
  md5fileNew.open ( temp_md5file.c_str());
  
  for(cii=files.begin(); cii!=files.end(); cii++)
    {
      string fn = *cii;
      bool is_up_to_date = false;           
      if(fileExists(fn)){
	const char* file = (*cii).c_str();
	const char*  md5 = computeMD5(file);
	string currentMd5s(md5);
	
	
	if(mapContains(md5values,fn)){
	  string prevMd5s = md5values[fn];
	  is_up_to_date = (currentMd5s == prevMd5s);
	}
	
	if(!is_up_to_date){
	  cout << file << " changed" << endl;
	}else{
	  cout << file << " unchanged" << endl;
	}
	
	md5fileNew << *cii << " " <<  currentMd5s <<endl;
      }
      FileStatus[*cii] =  is_up_to_date;
    }
  char cmd[512];
  sprintf(cmd,"cp %s %s",temp_md5file.c_str(),md5file.c_str()); 
  system(cmd); 
  md5fileNew.close();
  return;
}

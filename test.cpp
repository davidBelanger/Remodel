#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <openssl/md5.h>

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

char* computeMD5(const char* filename ){
  FILE *in = fopen (filename, "rb");
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

int main() {

  string filename = "ReMakeFile";
  
  vector<string> files = *getFiles(filename);
    
  vector<string>::const_iterator cii;
  for(cii=files.begin(); cii!=files.end(); cii++)
    {
      const char* file = (*cii).c_str();
      const char*  md5 = computeMD5(file);
      string md5s(md5);
      cout << *cii << " " <<md5s <<endl;
    }


  return 0;
}


#include <openssl/md5.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

void getFileStatuses(vector<string> files, map<string,bool>& FileStatus);

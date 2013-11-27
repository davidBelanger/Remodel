#include <openssl/md5.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/config.hpp> 
#include "boost/algorithm/string/erase.hpp"
#include "boost/algorithm/string/classification.hpp"

using namespace std;

void getFileStatuses(vector<string> files, map<string,bool>& FileStatus);

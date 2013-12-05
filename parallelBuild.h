#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "tbb/flow_graph.h"
#include <stdlib.h>
#include <functional>
#include "tbb/mutex.h"
#include "tbb/tbb_thread.h"

#include "fileParsing.h"

using namespace std;

void buildInParallel(StringToDepNodeMap dnMap,map<string,bool>& FileStatus);


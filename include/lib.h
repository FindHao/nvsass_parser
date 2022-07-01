#ifndef NVSASS_PARSER_LIB_H
#define NVSASS_PARSER_LIB_H

#include <fstream>
#include "FuncInfo.h"
#include "helpFunc.h"


using namespace std;

extern map<string, FuncInfo> map_FuncInfos;

void init(string path);

map<string, FuncInfo> mapOffset(string dataPath);

//void analyzeLines(stringstream *ss);

#endif //NVSASS_PARSER_LIB_H

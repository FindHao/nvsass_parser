#ifndef NVSASS_PARSER_PARSER_H
#define NVSASS_PARSER_PARSER_H

#include <fstream>
#include "FuncInfo.h"
#include "helpFunc.h"
#include "mapRes.pb.h"
#include "DataType.h"
using namespace std;

extern map<string, FuncInfo> map_FuncInfos;

void init(string path);

map<string, FuncInfo> mapOffset(string dataPath);

void decode(string serializedStr);
string encode(map<string, FuncInfo> map_FI);
void searchOffset_protobuf(kernel::mapRes::FuncInfo FI, int search_offset);
void regUsageParse(string &str_REG, Register &reg, string &funcName, int offset, string &sass_code);

//void analyzeLines(stringstream *ss);

#endif //NVSASS_PARSER_PARSER_H
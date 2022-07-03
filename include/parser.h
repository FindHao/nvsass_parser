#ifndef NVSASS_PARSER_PARSER_H
#define NVSASS_PARSER_PARSER_H

#include <fstream>
#include "FuncInfo.h"
#include "helpFunc.h"
#include "mapRes.pb.h"
#include "ArrayType.h"

using namespace std;

extern map<string, FuncInfo> map_FuncInfos;

void init(string &path);

map<string, FuncInfo> mapOffset(string &dataPath);

google::protobuf::Map<string, kernel::mapRes_FuncInfo> decode(string &serializedStr);

string encode(map<string, FuncInfo> map_FI);

void searchOffset_protobuf(const kernel::mapRes::FuncInfo &funcinfo, int search_offset);

void regUsageParse(string &str_REG, Register &reg, string &funcName, int offset, string &sass_code);

//void analyzeLines(stringstream *ss);

#endif //NVSASS_PARSER_PARSER_H

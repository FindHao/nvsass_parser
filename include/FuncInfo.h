#ifndef NVSASS_PARSER_FUNCINFO_H
#define NVSASS_PARSER_FUNCINFO_H

#include "SASSLineInfo.h"
#include <map>

using std::map;

class FuncInfo {
private:
    string funcName;
    set<string> srcFileSet;
    set<string> codeSet;
    map<int, struct SASSLineInfo> map_offset_src;
public:
    FuncInfo();
    explicit FuncInfo(string funcName);

    void setFuncName(string funcName);
    string getFuncName();
    void addSrcFile(string filePath);
    set<string> getSrcFile();
    // code set
    set<string> getCodeSet();
    // map offset and filePath, line, code
    void addOffsetSrc(int offset, string filePath, string line, string code, Register *reg_GPR, Register *reg_PRED,
                      Register *reg_UGPR, Register *reg_UPRED);

    map<int, struct SASSLineInfo> getOffsetSrc();
};


#endif //NVSASS_PARSER_FUNCINFO_H

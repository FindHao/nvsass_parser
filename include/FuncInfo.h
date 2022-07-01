#ifndef NVSASS_PARSER_FUNCINFO_H
#define NVSASS_PARSER_FUNCINFO_H

#include <string>
#include <map>
#include <set>
#include <vector>
#include "SASSLineInfo.h"

using namespace std;

class FuncInfo {
private:
    string funcName;
    set<string> srcFileSet; //换成 set  path
    set<string> codeSet;
    map<int, struct SASSLineInfo> map_offset_src; // map: offset -> srcfile name, line, code          存储指向srcFile的指针      偏移量 int

    int reg_GPR_size;
    int reg_PRED_size;
    int reg_UGPR_size;
    int reg_UPRED_size;
public:
    FuncInfo();
    FuncInfo(string funcName);


    // function name
    void setFuncName(string funcName);
    string getFuncName();

    // SrcFile
    void addSrcFile(string filePath);
    set<string> getSrcFile();

    // code set
    set<string> getCodeSet();

    // map offset and filePath, line, code
    void addOffsetSrc(int offset, string filePath, string line, string code, Register *reg_GPR, Register *reg_PRED, Register *reg_UGPR, Register *reg_UPRED);
    map<int, struct SASSLineInfo> getOffsetSrc();

    SASSLineInfo searchOffset(int offset);

    // Register
    void setRegister(string regName, int regSize);

    // Print
    void printSrcFile();
    void printOffset();
};


#endif //NVSASS_PARSER_FUNCINFO_H

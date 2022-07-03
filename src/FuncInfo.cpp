#include "FuncInfo.h"

using std::pair;

FuncInfo::FuncInfo() {
    this->funcName = "";
}

FuncInfo::FuncInfo(string funcName) {
    this->funcName = funcName;
}

// Function Name
void FuncInfo::setFuncName(string funcName) {
    this->funcName = funcName;
    //this->srcFileSet.insert("no src file");
}

string FuncInfo::getFuncName() {
    return this->funcName;
}

// Src File
void FuncInfo::addSrcFile(string filePath) {
    //this->srcFileSet.insert(filePath.append("     Line: ").append(fileLine));
    this->srcFileSet.insert(filePath);
}

set<string> FuncInfo::getSrcFile() {
    return this->srcFileSet;
}

// Code Set
set<string> FuncInfo::getCodeSet() {
    return this->codeSet;
}
void
FuncInfo::addOffsetSrc(int offset, string filePath, string line, string code, Register *reg_GPR, Register *reg_PRED,
                       Register *reg_UGPR, Register *reg_UPRED) {
    this->codeSet.insert(code); // 找找直接返回指针的 set 函数
    SASSLineInfo sassLine_info;
    sassLine_info.src_path = this->srcFileSet.find(filePath);
    sassLine_info.src_line = atoi(line.c_str());
    sassLine_info.code = this->codeSet.find(code);
    sassLine_info.reg_GPR = reg_GPR;
    sassLine_info.reg_PRED = reg_PRED;
    sassLine_info.reg_UGPR = reg_UGPR;
    sassLine_info.reg_UPRED = reg_UPRED;
    this->map_offset_src.insert(pair<int, struct SASSLineInfo>(offset, sassLine_info));

}

map<int, struct SASSLineInfo> FuncInfo::getOffsetSrc() {
    return this->map_offset_src;
}



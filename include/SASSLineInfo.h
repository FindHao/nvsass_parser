#ifndef NVSASS_PARSER_SASSLINEINFO_H
#define NVSASS_PARSER_SASSLINEINFO_H

#include <string>
#include <set>
#include "Register.h"

using namespace std;

struct SASSLineInfo{
    set<string>::iterator src_path;
    int src_line;
    set<string>::iterator code;
    //vector<Register> vec_reg;   //存储寄存器状态     //效率低，改成直接定义所有register如下
    Register *reg_GPR;
    Register *reg_PRED;
    Register *reg_UGPR;
    Register *reg_UPRED;
};

#endif //NVSASS_PARSER_SASSLINEINFO_H

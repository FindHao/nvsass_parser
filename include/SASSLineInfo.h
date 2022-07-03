#ifndef NVSASS_PARSER_SASSLINEINFO_H
#define NVSASS_PARSER_SASSLINEINFO_H

#include <string>
#include <set>
#include "Register.h"

using std::set;

struct SASSLineInfo {
    set<string>::iterator src_path;
    int src_line;
    set<string>::iterator code;
    Register *reg_GPR;
    Register *reg_PRED;
    Register *reg_UGPR;
    Register *reg_UPRED;
};

#endif //NVSASS_PARSER_SASSLINEINFO_H

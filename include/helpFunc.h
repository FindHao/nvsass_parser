#ifndef NVSASS_PARSER_HELPFUNC_H
#define NVSASS_PARSER_HELPFUNC_H

#include <iostream>
#include "FuncInfo.h"
#include <cstring>
#include <algorithm>


int hexToInt(string str);

vector<string> splitCode(string &code);

int regCount(string numStr);

bool ifContainsWide(string code);

string splitFuncType(string code);


const vector<string> AssembFunc = {
//      the followings are integer instructions.
        "IMAD",
        "IMADSP",
        "IMUL",
        "IADD",
        "ISCADD",
        "ISAD",
        "IMNMX",
        "BFE",
        "BFI",
        "SHR",
        "SHL",
        "SHF",
        "LOP",
        "FLO",
        "ISET",
        "ISETP",
        "ICMP",
        "POPC",
        "INT_END",
//      the following are FP32 instructions
        "FFMA",
        "FADD",
        "FCMP",
        "FMUL",
        "FMNMX",
        "FSWZ",
        "FSET",
        "FSETP",
        "FCHK",
        "RRO",
        "FP32_END",
//      the following are FP64 instructions
        "MUFU",
        "DFMA",
        "DADD",
        "DMUL",
        "DMNMX",
        "DSET",
        "DSETP",
        "FP64_END",
};


#endif //NVSASS_PARSER_HELPFUNC_H

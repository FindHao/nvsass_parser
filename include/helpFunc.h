#ifndef NVSASS_PARSER_HELPFUNC_H
#define NVSASS_PARSER_HELPFUNC_H
#include <iostream>
#include "FuncInfo.h"
#include <cstring>
#include <algorithm>
using namespace std;

int hexToInt(string str);

vector<string> splitCode(string code);
void searchOffset(FuncInfo FI, int search_offset);
int regCount(string numStr);
bool ifContainsWide(string code);
string splitFuncType(string code);


const vector<string> AssembFunc = {
//      the followings are integer instructions.
        "IMAD",
        "IMADSP",
        "IMUL",
        "IADD",
        "IADD3",
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
        "LEA",
        "ISCADD",
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
//      the following are FP64 instructions
        "MUFU",
        "DFMA",
        "DADD",
        "DMUL",
        "DMNMX",
        "DSET",
        "DSETP"
};


#endif //NVSASS_PARSER_HELPFUNC_H

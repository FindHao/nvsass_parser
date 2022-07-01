#include "FuncInfo.h"
#include <iostream>
#include <fstream>

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

void FuncInfo::addOffsetSrc(int offset, string filePath, string line, string code, Register *reg_GPR, Register *reg_PRED, Register *reg_UGPR, Register *reg_UPRED) {

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


SASSLineInfo FuncInfo::searchOffset(int offset) {
    //cout << "**************" << this->map_offset_src[0].reg_GPR->reg_status.size() << endl;

    auto iter = this->map_offset_src.find(offset);
    if (iter == map_offset_src.end()) {
        cout << "ERROR: Not exists this offset" << endl;
        SASSLineInfo emptySI;
        return emptySI;
    } else {
        /*

        cout << "Offset: " << offset << endl;
        cout << "   Src File Path: " << *iter->second.src_path << "   Line " << iter->second.src_line << endl;
        cout << "   Code: " << *iter->second.code << endl;

        cout << "   GPR status: ";
        vector<int> reg_status = iter->second.reg_GPR->reg_status;
        for (int i = 0; i < reg_status.size(); i++) {
            switch (reg_status[i]) {
                case 0:
                    break;
                case 1:
                    cout << "R" << i << " is " << "^" << "   ";
                    break;
                case 2:
                    cout << "R" << i << " is " << "v" << "   ";
                    break;
                case 3:
                    cout << "R" << i << " is " << "x" << "   ";
                    break;
                case 4:
                    cout << "R" << i << " is " << ":" << "   ";
                    break;
            }
        }
        cout << endl;

        cout << "   PRED status: ";
        reg_status = iter->second.reg_PRED->reg_status;
        for (int i = 0; i < reg_status.size(); i++) {
            switch (reg_status[i]) {
                case 0:
                    break;
                case 1:
                    cout << "R" << i << " is " << "^" << "   ";
                    break;
                case 2:
                    cout << "R" << i << " is " << "v" << "   ";
                    break;
                case 3:
                    cout << "R" << i << " is " << "x" << "   ";
                    break;
                case 4:
                    cout << "R" << i << " is " << ":" << "   ";
                    break;
            }
        }
        cout << endl;

        cout << "   UGPR status: ";
        reg_status = iter->second.reg_UGPR->reg_status;
        for (int i = 0; i < reg_status.size(); i++) {
            switch (reg_status[i]) {
                case 0:
                    break;
                case 1:
                    cout << "R" << i << " is " << "^" << "   ";
                    break;
                case 2:
                    cout << "R" << i << " is " << "v" << "   ";
                    break;
                case 3:
                    cout << "R" << i << " is " << "x" << "   ";
                    break;
                case 4:
                    cout << "R" << i << " is " << ":" << "   ";
                    break;
            }
        }
        cout << endl;

        cout << "   UPRED status: ";
        reg_status = iter->second.reg_UPRED->reg_status;
        for (int i = 0; i < reg_status.size(); i++) {
            switch (reg_status[i]) {
                case 0:
                    break;
                case 1:
                    cout << "R" << i << " is " << "^" << "   ";
                    break;
                case 2:
                    cout << "R" << i << " is " << "v" << "   ";
                    break;
                case 3:
                    cout << "R" << i << " is " << "x" << "   ";
                    break;
                case 4:
                    cout << "R" << i << " is " << ":" << "   ";
                    break;
            }
        }
        cout << endl << endl;

        */

        return iter->second;
    }

}





// set Register
void FuncInfo::setRegister(string regName, int regSize) {
    if (regName == "GPR") {
        this->reg_GPR_size = regSize;
    } else if (regName == "PRED"){
        this->reg_PRED_size = regSize;
    } else if (regName == "UGPR"){
        this->reg_UGPR_size = regSize;
    } else if (regName == "UPRED"){
        this->reg_UPRED_size = regSize;
    } else {
        cout << "No Such Register" << endl;
    }
}






// Print
void FuncInfo::printSrcFile() {
    for (auto i = this->srcFileSet.begin(); i != this->srcFileSet.end(); i++)
        cout << *i << endl;
}

void FuncInfo::printOffset() {
//    string s1("../data/printOffset_"), s2(this->funcName), s3(".txt");
//    ofstream outfile("../data/printOffset_");
//    if(!outfile){
//        cout << "Unable to open outfile";
//        exit(1); // terminate with error
//    }

    //map<int, struct SASSLineInfo> offset_src = this->getOffsetSrc();
    auto iter = this->map_offset_src.begin();
    while (iter != this->map_offset_src.end()) {
        //cout << this->funcName << endl;
        cout << "Offset: " << iter->first << endl;
        cout << "   Src File: " << *iter->second.src_path << "     Line: " << iter->second.src_line << endl;
        cout << "   Code: " << *iter->second.code << endl;

        cout << "   GPR status: ";
        vector<int> reg_status = iter->second.reg_GPR->reg_status;
        for (int i = 0; i < reg_status.size(); i++) {
            switch (reg_status[i]) {
                case 0:
                    break;
                case 1:
                    cout << "R" << i << " is " << "^" << "   ";
                    break;
                case 2:
                    cout << "R" << i << " is " << "v" << "   ";
                    break;
                case 3:
                    cout << "R" << i << " is " << "x" << "   ";
                    break;
                case 4:
                    cout << "R" << i << " is " << ":" << "   ";
                    break;
            }
        }
        cout << endl << endl;

//        outfile << this->funcName << "\n";
//        outfile << "Offset: " << iter->first << "\n";
//        outfile << "   Src File: " << *iter->second.src_path << "     Line: " << iter->second.src_line << "\n";
//        outfile << "   Code: " << *iter->second.code << "\n" << "\n";

        iter++;
    }

//    outfile.close();
}

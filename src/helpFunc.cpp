#include "helpFunc.h"
#include "DataType.h"
#include <sstream>
#include <vector>

using namespace std;


int hexToInt(string str) {
    int len = str.size();
    int res = 0;

    for (int i = 0; i < len; i++) {
        if (str[i] >= 'a' && str[i] <= 'f')
            res = (10 + str[i] - 'a') + res * 16;
        else
            res = (str[i] - '0') + res * 16;
    }

    return res;
}

vector<string> splitCode(string code) { // 后续考虑并入类中             // 找找 C++ regex 中 split
    string str;
    vector<string> vec_code;

    for (int i = 0; i < strlen(code.c_str()); i++) {
        if (str.empty()) {
            if (code[i] == ',' || code[i] == ' ' || code[i] == ';')
                continue;
            else {
                str.append(1, code[i]);
            }
        } else {
            if (code[i] == ',' || code[i] == ' ' || code[i] == ';') {
                if (str[0] != '@')
                    vec_code.push_back(str);
                str = "";
            } else {
                str.append(1, code[i]);
                if (i == strlen(code.c_str()) - 1)
                    vec_code.push_back(str);
            }
        }
    }

    for (int i = 0; i < vec_code.size(); i++) {
        cout << vec_code[i] << endl;
    }

    return vec_code;
}

// 参数 offset, kernel name
void searchOffset(FuncInfo FI, int search_offset) {
    DataType resType;
    SASSLineInfo SI = FI.searchOffset(search_offset);
    Register *reg_GPR = SI.reg_GPR;
    string code = *SI.code;
    //cout << code << endl << endl;
    vector<string> vec_code = splitCode(code);

    string funcType = splitFuncType(vec_code[0]);   // split by '.',  get the "LDG" from "LDG.E.64"
    if (funcType == "LDG" || funcType == "LD") {   // Load         // 改成 "LDG" 在不在 string 里    // 已知 offset       // store  "ST"
        cout << endl << "Load：" << endl;

        // find which reg is used (writen)
        string get_reg = vec_code[1]; // "LDG.E.64 R8, [R16.64] ;" 中的 "R8"
        int reg_write = atoi(get_reg.substr(1).c_str());
        if (reg_write < 0 || reg_write >= reg_GPR->size) {
            cout << "ERROR: Index of LDG wrong." << endl;
        }

//        int reg_write = -1;
//        for (int i = 0; i < reg_GPR->reg_status.size(); i++) {
//            if ((reg_GPR->reg_status[i] & 0x1) == 0x1) { // 1 -> 写 ^, 3 -> 读+写 x     // reg_GPR.reg_status[i] & 0x1  // 增加 map<1, reg4> 两者都用
//                reg_write = i;
//                break;
//            }
//        }
//
//        // if reg_write == -1  error
//        if (reg_write == -1) {
//            cout << "ERROR: Not found any register is writen after loading the data." << endl;
//        }



        // find where this reg is used (read)
        bool found = false;
        for (search_offset += 0x10; search_offset <= (FI.getOffsetSrc().size() - 1) * 0x10; search_offset += 0x10) { // 16 -> 0x10
            SI = FI.searchOffset(search_offset);
            reg_GPR = SI.reg_GPR;
            if ((reg_GPR->reg_status[reg_write] & 0x2) == 0x2) { // 2 -> 读 v, 3 -> 读+写 x     // & 0x2
                found = true;
                break;  // find the read line
            }
        }

        // if 没找到  error
        if (!found) {
            cout << "ERROR: Not found where the register just written is read after loading process." << endl;
        }

        code = *SI.code;
        vec_code = splitCode(code);


        // enum / map
        auto iter = std::find(AssembFunc.begin(), AssembFunc.end(), splitFuncType(vec_code[0]));
        if (iter != AssembFunc.end()) {
            int index = distance(AssembFunc.begin(), iter);     // iter - begin
            cout << "index: " << index << endl;
            if (index >= 28) {
                cout << "Type is FP64" << endl;        // 类型建structure  1. char type  0-> int  1-> float  2. 几bit的类型 32-> FP32  16-> FP64      返回这个结构体
            } else if (index < 28 && index >= 18) {
                cout << "Type is FP32" << endl;
            } else { // index < 18
                cout << "Type is INT" << endl;
            }
        } else {
            // error
            cout << "ERROR: Not found this function in the set." << endl;
        }

    } else if (funcType == "STG" || funcType == "ST") {    // Store
        cout << endl << "Store：" << endl;

        // find which reg is used (read)
        string get_reg = vec_code[2];  // "STG.E.64 [R14.64], R8;" 的 R8
        int reg_getData = atoi(get_reg.substr(1).c_str());
        if (reg_getData < 0 || reg_getData >= reg_GPR->size) {
            cout << "ERROR: Index of STG wrong." << endl;
        }

        // find when this reg is written
        bool found = false;
        for (search_offset -= 0x10; search_offset >= 0x00; search_offset -= 0x10) { // 16 -> 0x10
            SI = FI.searchOffset(search_offset);
            reg_GPR = SI.reg_GPR;
            if ((reg_GPR->reg_status[reg_getData] & 0x1) == 0x1) { // 1 -> 写 ^, 3 -> 读+写 x
                found = true;
                break;  // find the read line
            }
        }

        // if 没找到  error
        if (!found) {
            cout << "ERROR: Not found where the register just read is written before storing process." << endl;
        }

        code = *SI.code;
        vec_code = splitCode(code);

        auto iter = std::find(AssembFunc.begin(), AssembFunc.end(), splitFuncType(vec_code[0]));
        if (iter != AssembFunc.end()) {
            int index = distance(AssembFunc.begin(), iter);     // iter - begin
            cout << "index: " << index << endl;
            if (index >= 28) {
                cout << "Type is FP64" << endl;        // 类型建structure  1. char type  0-> int  1-> float  2. 几bit的类型 32-> FP32  16-> FP64      返回这个结构体
                resType.type = 1;
                resType.version = 64;
            } else if (index < 28 && index >= 18) {
                cout << "Type is FP32" << endl;
                resType.type = 1;
                resType.version = 32;
            } else { // index < 18
                cout << "Type is INT" << endl;
                resType.type = 0;
                resType.version = 0;
            }
        } else {
            // error
            cout << "ERROR: Not found this function in the set." << endl;
        }
    } else {
        cout << "Not LD/LDG/ST/STG" << endl;
    }

    cout << "===============================================" << endl;
}

// 计算reg个数的辅助函数
int regCount(string numStr) {
    stringstream input(numStr);
    string num;
    while (input>>num)
        continue;

    return atoi(num.c_str()) + 1;
}

bool ifContainsWide(string code) {
    stringstream sscode(code);
    string token;
    while (getline(sscode, token, '.')) {
        if (token == "WIDE")
            return true;
    }
    return false;
}

string splitFuncType(string code) { // like "ISETP.GE.AND"
    stringstream sscode(code);
    string token;
    if (getline(sscode, token, '.'))
        return token;
    else
        return code;
}



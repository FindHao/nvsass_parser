#include "helpFunc.h"
#include "ArrayType.h"
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

vector<string> splitCode(string &code) {
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

// 计算reg个数的辅助函数
int regCount(string numStr) {
    stringstream input(numStr);
    string num;
    while (input >> num)
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



#include <iostream>
#include <string>
#include "parser.h"
#include <sstream>

using namespace std;


int main(int argc, char* argv[]) {

    init("../data/castro1.txt");
    cout << "Test Name: " << map_FuncInfos.begin()->second.getFuncName() << endl;
//    cout << "__cuda_sm20_div_f64_slowpath_v2: " << map_FuncInfos["__cuda_sm20_div_f64_slowpath_v2"].searchOffset(0).src_line << endl;

    string serializedStr = encode(map_FuncInfos);

    ofstream OsWrite("store.txt");
    OsWrite << serializedStr;
    OsWrite.close();
/*----------------上面是序列化，下面是反序列化-----------------------*/
    ifstream IsRead;
    IsRead.open("store.txt");
    string strToDecode = "";
//    auto ss = std::ostringstream();
    ostringstream ss;
    ss<<IsRead.rdbuf();
    strToDecode = ss.str();

    decode(strToDecode);

    return 0;
}

#include "lib.h"
#include <sstream>
#include <cstdio>

using namespace std;

map<string, FuncInfo> map_FuncInfos;
vector<stringstream*> vec_ss;

void init(string path) {
    map_FuncInfos = mapOffset(path);
}

map<string, FuncInfo> mapOffset(string dataPath) {

    string tempStr;
    ifstream myfile;
//    ofstream outfile("../data/result2.txt");

    // Buffer
    char buf[64 * 1024];
    myfile.rdbuf()->pubsetbuf(buf, sizeof buf);
    myfile.open(dataPath);

    if(!myfile){
        cout << "Unable to open myfile";
        exit(1); // terminate with error
    }
//    if(!outfile){
//        cout << "Unable to open otfile";
//        exit(1); // terminate with error
//    }




//    string lineStr;
//    stringstream *ss = new stringstream(lineStr);
//    while (getline(myfile, lineStr)) {
//        if (lineStr.find("//-----") != tempStr.npos) {
//            vec_ss.push_back(new stringstream(ss->str()));
//            ss->clear();
//            ss->str("");
//        }
//
//        *ss << lineStr << "\n";
//
//    }
//    vec_ss.push_back(new stringstream(ss->str()));
//
////    #pragma omp parallel for shared(map_FuncInfos)
//    for (int i = 0; i < vec_ss.size(); i++) {
//        analyzeLines(vec_ss[i]);
//    }

    FILE *fp;
    fp = fopen(dataPath.c_str(),"r");

//    while (!feof(fp)) {
//        fgets(buf, sizeof buf, fp);
//        tempStr = buf;
//    }


    // Analyze line by line
    FuncInfo *FI = nullptr;
//    map<string, FuncInfo> map_FuncInfos; // function name -> FuncInfo Objects
//    vector<FuncInfo> vec_FuncInfos;

    string filePath = "no src file";
    string fileLine = "-1";
    int reg_GPR_size = -1;
    int reg_PRED_size = -1;
    int reg_UGPR_size = -1;
    int reg_UPRED_size = -1;
    while (!feof(fp)) {  // 文件整个先读进memory(ifstream stringstream)   切割不同 kernel function，每一块作为input传进： while换成function -》 参数  getline(myfile, tempStr)
        fgets(buf, sizeof buf, fp);
        tempStr = buf;

        if (tempStr.empty())
            continue;
        tempStr.erase(0, tempStr.find_first_not_of(" \t"));   // find_first_of("/")       不用erase 不新建字符串 substr
        if (tempStr[0] == '.')
            continue;
        // if (tempStr[0] == '/' && tempStr.find("#") == tempStr.npos)

        if (tempStr[1] == '*' && tempStr[0] == '/') {    // if (tempStr.find("/*") != tempStr.npos)
            // match offset and assembly code
//            vector<string> offset_code = getMatch("/\\*(.*)\\*/( +)(.*); (.*)\\/\\/ \\|\\s*(.*)\\|\\s*(.*)\\|\\s*(.*)\\|\\s*(.*)\\|", tempStr);
//            if (!offset_code.empty()) {
//            }

            FI->addSrcFile(filePath);     //解决没有源文件情形，不知道为什么加在前面会报139错

            int offset = hexToInt(tempStr.substr(2, tempStr.find("*/") - 2));       // 注意 find 和 find_first_of 的区别

            // erase 汇编代码之前所有内容
            tempStr.erase(0, tempStr.find_first_of(" "));
            tempStr.erase(0, tempStr.find_first_not_of(" "));

            string code = tempStr.substr(0, tempStr.find_first_of(";"));
            //outfile << "Offset: " << offset << "       Assembly Code: " << code << "\n";

            // erase 寄存器个数之前所有内容
            tempStr.erase(0, tempStr.find_first_of("//"));
            tempStr.erase(0, tempStr.find_first_of("|") + 1);
            tempStr.erase(0, tempStr.find_first_not_of(" "));

            // construct register
            auto *reg_GPR = new Register();
            reg_GPR->size = reg_GPR_size;
            reg_GPR->name = "GPR";
            auto *reg_PRED = new Register();
            reg_PRED->size = reg_PRED_size;
            reg_PRED->name = "PRED";
            auto *reg_UGPR = new Register();
            reg_UGPR->size = reg_UGPR_size;
            reg_UGPR->name = "UGPR";
            auto *reg_UPRED = new Register();
            reg_UPRED->size = reg_UPRED_size;
            reg_UPRED->name = "UPRED";
            //vector<string> reg_status = getMatch("(.*)\\/\\/ \\|\\s*(.*)\\|\\s*(.*)\\|\\s*(.*)\\|\\s*(.*)\\|", tempStr);  // (.*)\/\/ \|\s*(.*)\|\s*(.*)\|    \s* 取代空格
            string str_GPR = tempStr.substr(0, tempStr.find_first_of("|"));
            tempStr.erase(0, tempStr.find_first_of("|") + 1);
            tempStr.erase(0, tempStr.find_first_not_of(" "));

            string str_PRED = tempStr.substr(0, tempStr.find_first_of("|"));
            tempStr.erase(0, tempStr.find_first_of("|") + 1);
            tempStr.erase(0, tempStr.find_first_not_of(" "));

            string str_UGPR = "";
            string str_UPRED = "";
            int tempPos = tempStr.find_first_of("|");
            if (tempPos != tempStr.npos) {
                str_UGPR = tempStr.substr(0, tempPos);
                tempStr.erase(0, tempPos + 1);
                tempStr.erase(0, tempStr.find_first_not_of(" "));

                tempPos = tempStr.find_first_of("|");
                if (tempPos != tempStr.npos) {
                    str_UPRED = tempStr.substr(0, tempStr.find_first_of("|"));
                }
            }





            if (str_GPR.empty()) {  // no reg_GPR used now
                reg_GPR->occupied_count = 0;
                vector<int> reg_s(reg_GPR->size, 0);
                reg_GPR->reg_status = reg_s;
                //cout << "GPR empty" << endl;
            } else {
                int index = str_GPR.find(' ') - 1;  // active number 的末位
                reg_GPR->occupied_count = atoi(str_GPR.substr(0,index + 1).c_str());
                //cout << "GPR occupied_count: " << reg_GPR->occupied_count << endl;
                for (int i = 0; i < reg_GPR->size; i++) {
                    index = index + 1 + to_string(i).size();
                    //cout << str_GPR[index] << endl;
                    switch (str_GPR[index]) {
                        case ' ':
                            reg_GPR->reg_status.push_back(0);
                            break;
                        case '^':
                            reg_GPR->reg_status.push_back(1);
                            break;
                        case 'v':
                            reg_GPR->reg_status.push_back(2);
                            break;
                        case 'x':
                            reg_GPR->reg_status.push_back(3);
                            break;
                        case ':':
                            reg_GPR->reg_status.push_back(4);
                            break;
                        default:
                            cout << "Error: Unidentify: " << str_GPR[index] << endl;
                            cout << "FuncName: " << FI->getFuncName() << endl;
                            cout << "Offset: " << offset << endl;
                            cout << "Code: " << code << endl;
                            exit(0);
                    }
                }
            }

            if (str_PRED.empty()) {  // no reg_PRED used now
                reg_PRED->occupied_count = 0;
                vector<int> reg_s(reg_PRED->size, 0);
                reg_PRED->reg_status = reg_s;
            } else {
                int index = str_PRED.find(' ') - 1;  // active number 的末位
                reg_PRED->occupied_count = atoi(str_PRED.substr(0,index + 1).c_str());
                for (int i = 0; i < reg_PRED->size; i++) {
                    index = index + 1 + to_string(i).size();
                    switch (str_PRED[index]) {
                        case ' ':
                            reg_PRED->reg_status.push_back(0);
                            break;
                        case '^':
                            reg_PRED->reg_status.push_back(1);
                            break;
                        case 'v':
                            reg_PRED->reg_status.push_back(2);
                            break;
                        case 'x':
                            reg_PRED->reg_status.push_back(3);
                            break;
                        case ':':
                            reg_PRED->reg_status.push_back(4);
                            break;
                        default:
                            cout << "Error: Unidentify:" << str_PRED[index] << endl;
                    }
                }
            }

            if (str_UGPR.empty()) {  // no reg_UGPR used now
                reg_UGPR->occupied_count = 0;
                vector<int> reg_s(reg_UGPR->size, 0);
                reg_UGPR->reg_status = reg_s;
            } else {
                int index = str_UGPR.find(' ') - 1;  // active number 的末位
                reg_UGPR->occupied_count = atoi(str_UGPR.substr(0,index + 1).c_str());
                for (int i = 0; i < reg_UGPR->size; i++) {
                    index = index + 1 + to_string(i).size();
                    switch (str_UGPR[index]) {
                        case ' ':
                            reg_UGPR->reg_status.push_back(0);
                            break;
                        case '^':
                            reg_UGPR->reg_status.push_back(1);
                            break;
                        case 'v':
                            reg_UGPR->reg_status.push_back(2);
                            break;
                        case 'x':
                            reg_UGPR->reg_status.push_back(3);
                            break;
                        case ':':
                            reg_UGPR->reg_status.push_back(4);
                            break;
                        default:
                            cout << "Error: Unidentify:" << str_UGPR[index] << endl;
                    }
                }
            }

            if (str_UPRED.empty()) {  // no reg_UPRED used now
                reg_UPRED->occupied_count = 0;
                vector<int> reg_s(reg_UPRED->size, 0);
                reg_UPRED->reg_status = reg_s;
            } else {
                int index = str_UPRED.find(' ') - 1;  // active number 的末位
                reg_UPRED->occupied_count = atoi(str_UPRED.substr(0,index + 1).c_str());
                for (int i = 0; i < reg_UPRED->size; i++) {
                    index = index + 1 + to_string(i).size();
                    switch (str_UPRED[index]) {
                        case ' ':
                            reg_UPRED->reg_status.push_back(0);
                            break;
                        case '^':
                            reg_UPRED->reg_status.push_back(1);
                            break;
                        case 'v':
                            reg_UPRED->reg_status.push_back(2);
                            break;
                        case 'x':
                            reg_UPRED->reg_status.push_back(3);
                            break;
                        case ':':
                            reg_UPRED->reg_status.push_back(4);
                            break;
                        default:
                            cout << "Error: Unidentify:" << str_UPRED[index] << endl;
                    }
                }
            }

            //  add to the object
            FI->addOffsetSrc(offset, filePath, fileLine, code, reg_GPR, reg_PRED, reg_UGPR, reg_UPRED);

            continue;

        } else if (tempStr[2] == '-') {
            // match function name
//            vector<string> function_name = getMatch("//-+ \\.text\\.(.*) -+", tempStr);
//            if (!function_name.empty()) {   // match 到 function 了
//            }

            if (FI != nullptr) {
                //vec_FuncInfos.push_back(*FI);
                if (map_FuncInfos.find(FI->getFuncName()) != map_FuncInfos.end())
                    cout << "ERROR: Kernel Exists!" << endl;
                map_FuncInfos.insert(pair<string, FuncInfo>(FI->getFuncName(), *FI));
                //cout << "test: " << map_FuncInfos[FI->getFuncName()].getFuncName() << endl;
            }

            tempStr.erase(0, tempStr.find_first_of("."));
            string kernel_str = tempStr.substr(6, tempStr.find_first_of(" ---") - 6);
            FuncInfo *tempObj = new FuncInfo(kernel_str);  // 改存 stack 为 heap
            FI = tempObj;
            filePath = "no src file";
            fileLine = "-1";

            reg_GPR_size = -1;
            reg_PRED_size = -1;
            reg_UGPR_size = -1;
            reg_UPRED_size = -1;
            //FI->addSrcFile(filePath, fileLine);       //报139错

            //cout << "Create: " << FI->getFuncName() << endl;
            continue;

        } else if (tempStr[2] == '#' && tempStr[1] == '/') {
            // match src file and corresponding line
            tempStr.erase(0, tempStr.find_first_of("\"") + 1);
            filePath = tempStr.substr(0, tempStr.find_first_of("\""));

            tempStr.erase(0, tempStr.find(", line") + 6);
            tempStr.erase(0, tempStr.find_first_not_of(" "));
            fileLine = tempStr.substr(0, tempStr.find_first_of(" "));

//            cout << "   Source File    Name: " << filePath << "       Line: " << fileLine << endl;
            FI->addSrcFile(filePath);

            continue;
        }else if (tempStr.find("#") != tempStr.npos && tempStr.find("// |") != tempStr.npos) {
            // count each register

            tempStr.erase(0, tempStr.find_first_of("#") + 1);
            reg_GPR_size = regCount(tempStr.substr(0, tempStr.find_first_of("|")));

            tempStr.erase(0, tempStr.find_first_of("#") + 1);
            reg_PRED_size = regCount(tempStr.substr(0, tempStr.find_first_of("|")));

            int tempPos = tempStr.find_first_of("#");
            if (tempPos != tempStr.npos) {
                tempStr.erase(0, tempPos + 1);
                reg_UGPR_size = regCount(tempStr.substr(0, tempStr.find_first_of("|")));

                tempPos = tempStr.find_first_of("#");
                if (tempPos != tempStr.npos) {
                    tempStr.erase(0, tempStr.find_first_of("#") + 1);
                    reg_UPRED_size = regCount(tempStr.substr(0, tempStr.find_first_of("|")));
                } else {
                    reg_UPRED_size = 0;
                }
            } else {
                reg_UGPR_size = 0;
            }
            //cout << "reg_GPR_size: " << reg_GPR_size << endl;
            //cout << "reg_PRED_size: " << reg_PRED_size << endl;
            //cout << "reg_UGPR_size: " << reg_UGPR_size << endl;
            //cout << "reg_UPRED_size: " << reg_UPRED_size << endl;

        } else {
            // match src file and corresponding line
//            vector<string> src_file = getMatch("(\\t*)\\/\\/## File \"(.*)\", line ([0-9]*)(.*)", tempStr);     //补上 else 排除
//            if (!src_file.empty()) {
//                filePath = src_file[1];
//                fileLine = src_file[2];
//                cout << "   Source File    Name: " << filePath << "       Line: " << fileLine << endl;
//                FI->addSrcFile(filePath, fileLine);
//
//                continue;
//            }




            // count each register          // 写进function的else
//            vector<string> reg_count = getMatch("(.*)\\/\\/ \\|\\s*#(.*)\\s+\\|\\s*#(.*)\\s+\\|\\s*#(.*)\\s+\\|\\s*#(.*)\\s+\\|", tempStr);
//            if (!reg_count.empty()) {
//                reg_GPR_size = regCount(reg_count[1]);
//                //cout << "reg_GPR_size: " << reg_GPR_size << endl;
//                reg_PRED_size = regCount(reg_count[2]);
//                //cout << "reg_PRED_size: " << reg_PRED_size << endl;
//                reg_UGPR_size = regCount(reg_count[3]);
//                //cout << "reg_UGPR_size: " << reg_UGPR_size << endl;
//                reg_UPRED_size = regCount(reg_count[4]);
//                //cout << "reg_UPRED_size: " << reg_UPRED_size << endl;
//
//                continue;
//            }
        }

    }

    if (FI != nullptr) {
        //vec_FuncInfos.push_back(*FI);
        map_FuncInfos.insert(pair<string, FuncInfo>(FI->getFuncName(), *FI));
    }







    myfile.close();
    fclose(fp);

    return map_FuncInfos;
}









//void analyzeLines(stringstream *ss) {
//    string tempStr;
//    FuncInfo *FI = nullptr;
//    string filePath = "no src file";
//    string fileLine = "-1";
//    int reg_GPR_size = -1;
//    int reg_PRED_size = -1;
//    int reg_UGPR_size = -1;
//    int reg_UPRED_size = -1;
//
//    while (getline(*ss, tempStr, '\n')) {
//        if (tempStr.empty())
//            continue;
//        tempStr.erase(0, tempStr.find_first_not_of(" \t"));   // find_first_of("/")       不用erase 不新建字符串 substr
//        if (tempStr[0] == '.')
//            continue;
//        // if (tempStr[0] == '/' && tempStr.find("#") == tempStr.npos)
//
//        if (tempStr[1] == '*' && tempStr[0] == '/') {    // if (tempStr.find("/*") != tempStr.npos)
//            // match offset and assembly code
////            vector<string> offset_code = getMatch("/\\*(.*)\\*/( +)(.*); (.*)\\/\\/ \\|\\s*(.*)\\|\\s*(.*)\\|\\s*(.*)\\|\\s*(.*)\\|", tempStr);
////            if (!offset_code.empty()) {
////            }
//
//            FI->addSrcFile(filePath, fileLine);     //解决没有源文件情形，不知道为什么加在前面会报139错
//
//            int offset = hexToInt(tempStr.substr(2, tempStr.find("*/") - 2));       // 注意 find 和 find_first_of 的区别
//
//            // erase 汇编代码之前所有内容
//            tempStr.erase(0, tempStr.find_first_of(" "));
//            tempStr.erase(0, tempStr.find_first_not_of(" "));
//
//            string code = tempStr.substr(0, tempStr.find_first_of(";"));
//            //outfile << "Offset: " << offset << "       Assembly Code: " << code << "\n";
//
//            // erase 寄存器个数之前所有内容
//            tempStr.erase(0, tempStr.find_first_of("//"));
//            tempStr.erase(0, tempStr.find_first_of("|") + 1);
//            tempStr.erase(0, tempStr.find_first_not_of(" "));
//
//            // construct register
//            auto *reg_GPR = new Register();
//            reg_GPR->size = reg_GPR_size;
//            reg_GPR->name = "GPR";
//            auto *reg_PRED = new Register();
//            reg_PRED->size = reg_PRED_size;
//            reg_PRED->name = "PRED";
//            auto *reg_UGPR = new Register();
//            reg_UGPR->size = reg_UGPR_size;
//            reg_UGPR->name = "UGPR";
//            auto *reg_UPRED = new Register();
//            reg_UPRED->size = reg_UPRED_size;
//            reg_UPRED->name = "UPRED";
//            //vector<string> reg_status = getMatch("(.*)\\/\\/ \\|\\s*(.*)\\|\\s*(.*)\\|\\s*(.*)\\|\\s*(.*)\\|", tempStr);  // (.*)\/\/ \|\s*(.*)\|\s*(.*)\|    \s* 取代空格
//            string str_GPR = tempStr.substr(0, tempStr.find_first_of("|"));
//            tempStr.erase(0, tempStr.find_first_of("|") + 1);
//            tempStr.erase(0, tempStr.find_first_not_of(" "));
//
//            string str_PRED = tempStr.substr(0, tempStr.find_first_of("|"));
//            tempStr.erase(0, tempStr.find_first_of("|") + 1);
//            tempStr.erase(0, tempStr.find_first_not_of(" "));
//
//            string str_UGPR = "";
//            string str_UPRED = "";
//            int tempPos = tempStr.find_first_of("|");
//            if (tempPos != tempStr.npos) {
//                str_UGPR = tempStr.substr(0, tempPos);
//                tempStr.erase(0, tempPos + 1);
//                tempStr.erase(0, tempStr.find_first_not_of(" "));
//
//                tempPos = tempStr.find_first_of("|");
//                if (tempPos != tempStr.npos) {
//                    str_UPRED = tempStr.substr(0, tempStr.find_first_of("|"));
//                }
//            }
//
//
//
//
//
//            if (str_GPR.empty()) {  // no reg_GPR used now
//                reg_GPR->occupied_count = 0;
//                vector<int> reg_s(reg_GPR->size, 0);
//                reg_GPR->reg_status = reg_s;
//                //cout << "GPR empty" << endl;
//            } else {
//                int index = str_GPR.find(' ') - 1;  // active number 的末位
//                reg_GPR->occupied_count = atoi(str_GPR.substr(0,index + 1).c_str());
//                //cout << "GPR occupied_count: " << reg_GPR->occupied_count << endl;
//                for (int i = 0; i < reg_GPR->size; i++) {
//                    index = index + 1 + to_string(i).size();
//                    //cout << str_GPR[index] << endl;
//                    switch (str_GPR[index]) {
//                        case ' ':
//                            reg_GPR->reg_status.push_back(0);
//                            break;
//                        case '^':
//                            reg_GPR->reg_status.push_back(1);
//                            break;
//                        case 'v':
//                            reg_GPR->reg_status.push_back(2);
//                            break;
//                        case 'x':
//                            reg_GPR->reg_status.push_back(3);
//                            break;
//                        case ':':
//                            reg_GPR->reg_status.push_back(4);
//                            break;
//                        default:
//                            cout << "Error: Unidentify: " << str_GPR[index] << endl;
//                            cout << "FuncName: " << FI->getFuncName() << endl;
//                            cout << "Offset: " << offset << endl;
//                            cout << "Code: " << code << endl;
//                            exit(0);
//                    }
//                }
//            }
//
//            if (str_PRED.empty()) {  // no reg_PRED used now
//                reg_PRED->occupied_count = 0;
//                vector<int> reg_s(reg_PRED->size, 0);
//                reg_PRED->reg_status = reg_s;
//            } else {
//                int index = str_PRED.find(' ') - 1;  // active number 的末位
//                reg_PRED->occupied_count = atoi(str_PRED.substr(0,index + 1).c_str());
//                for (int i = 0; i < reg_PRED->size; i++) {
//                    index = index + 1 + to_string(i).size();
//                    switch (str_PRED[index]) {
//                        case ' ':
//                            reg_PRED->reg_status.push_back(0);
//                            break;
//                        case '^':
//                            reg_PRED->reg_status.push_back(1);
//                            break;
//                        case 'v':
//                            reg_PRED->reg_status.push_back(2);
//                            break;
//                        case 'x':
//                            reg_PRED->reg_status.push_back(3);
//                            break;
//                        case ':':
//                            reg_PRED->reg_status.push_back(4);
//                            break;
//                        default:
//                            cout << "Error: Unidentify:" << str_PRED[index] << endl;
//                    }
//                }
//            }
//
//            if (str_UGPR.empty()) {  // no reg_UGPR used now
//                reg_UGPR->occupied_count = 0;
//                vector<int> reg_s(reg_UGPR->size, 0);
//                reg_UGPR->reg_status = reg_s;
//            } else {
//                int index = str_UGPR.find(' ') - 1;  // active number 的末位
//                reg_UGPR->occupied_count = atoi(str_UGPR.substr(0,index + 1).c_str());
//                for (int i = 0; i < reg_UGPR->size; i++) {
//                    index = index + 1 + to_string(i).size();
//                    switch (str_UGPR[index]) {
//                        case ' ':
//                            reg_UGPR->reg_status.push_back(0);
//                            break;
//                        case '^':
//                            reg_UGPR->reg_status.push_back(1);
//                            break;
//                        case 'v':
//                            reg_UGPR->reg_status.push_back(2);
//                            break;
//                        case 'x':
//                            reg_UGPR->reg_status.push_back(3);
//                            break;
//                        case ':':
//                            reg_UGPR->reg_status.push_back(4);
//                            break;
//                        default:
//                            cout << "Error: Unidentify:" << str_UGPR[index] << endl;
//                    }
//                }
//            }
//
//            if (str_UPRED.empty()) {  // no reg_UPRED used now
//                reg_UPRED->occupied_count = 0;
//                vector<int> reg_s(reg_UPRED->size, 0);
//                reg_UPRED->reg_status = reg_s;
//            } else {
//                int index = str_UPRED.find(' ') - 1;  // active number 的末位
//                reg_UPRED->occupied_count = atoi(str_UPRED.substr(0,index + 1).c_str());
//                for (int i = 0; i < reg_UPRED->size; i++) {
//                    index = index + 1 + to_string(i).size();
//                    switch (str_UPRED[index]) {
//                        case ' ':
//                            reg_UPRED->reg_status.push_back(0);
//                            break;
//                        case '^':
//                            reg_UPRED->reg_status.push_back(1);
//                            break;
//                        case 'v':
//                            reg_UPRED->reg_status.push_back(2);
//                            break;
//                        case 'x':
//                            reg_UPRED->reg_status.push_back(3);
//                            break;
//                        case ':':
//                            reg_UPRED->reg_status.push_back(4);
//                            break;
//                        default:
//                            cout << "Error: Unidentify:" << str_UPRED[index] << endl;
//                    }
//                }
//            }
//
//            //  add to the object
//            FI->addOffsetSrc(offset, filePath, fileLine, code, reg_GPR, reg_PRED, reg_UGPR, reg_UPRED);
//
//            continue;
//
//        } else if (tempStr[2] == '-') {
//            // match function name
////            vector<string> function_name = getMatch("//-+ \\.text\\.(.*) -+", tempStr);
////            if (!function_name.empty()) {   // match 到 function 了
////            }
//
//            if (FI != nullptr) {
//                //vec_FuncInfos.push_back(*FI);
//                if (map_FuncInfos.find(FI->getFuncName()) != map_FuncInfos.end())
//                    cout << "ERROR: Kernel Exists!" << endl;
//                map_FuncInfos.insert(pair<string, FuncInfo>(FI->getFuncName(), *FI));
//                //cout << "test: " << map_FuncInfos[FI->getFuncName()].getFuncName() << endl;
//            }
//
//            tempStr.erase(0, tempStr.find_first_of("."));
//            string kernel_str = tempStr.substr(6, tempStr.find_first_of(" ---") - 6);
//            FuncInfo *tempObj = new FuncInfo(kernel_str);  // 改存 stack 为 heap
//            FI = tempObj;
//            filePath = "no src file";
//            fileLine = "-1";
//
//            reg_GPR_size = -1;
//            reg_PRED_size = -1;
//            reg_UGPR_size = -1;
//            reg_UPRED_size = -1;
//            //FI->addSrcFile(filePath, fileLine);       //报139错
//
//            cout << "Create: " << FI->getFuncName() << endl;
//            continue;
//
//        } else if (tempStr[2] == '#' && tempStr[1] == '/') {
//            // match src file and corresponding line
//            tempStr.erase(0, tempStr.find_first_of("\"") + 1);
//            filePath = tempStr.substr(0, tempStr.find_first_of("\""));
//
//            tempStr.erase(0, tempStr.find(", line") + 6);
//            tempStr.erase(0, tempStr.find_first_not_of(" "));
//            fileLine = tempStr.substr(0, tempStr.find_first_of(" "));
//
////            cout << "   Source File    Name: " << filePath << "       Line: " << fileLine << endl;
//            FI->addSrcFile(filePath, fileLine);
//
//            continue;
//        }else if (tempStr.find("#") != tempStr.npos && tempStr.find("// |") != tempStr.npos) {
//            // count each register
//
//            tempStr.erase(0, tempStr.find_first_of("#") + 1);
//            reg_GPR_size = regCount(tempStr.substr(0, tempStr.find_first_of("|")));
//
//            tempStr.erase(0, tempStr.find_first_of("#") + 1);
//            reg_PRED_size = regCount(tempStr.substr(0, tempStr.find_first_of("|")));
//
//            int tempPos = tempStr.find_first_of("#");
//            if (tempPos != tempStr.npos) {
//                tempStr.erase(0, tempPos + 1);
//                reg_UGPR_size = regCount(tempStr.substr(0, tempStr.find_first_of("|")));
//
//                tempPos = tempStr.find_first_of("#");
//                if (tempPos != tempStr.npos) {
//                    tempStr.erase(0, tempStr.find_first_of("#") + 1);
//                    reg_UPRED_size = regCount(tempStr.substr(0, tempStr.find_first_of("|")));
//                } else {
//                    reg_UPRED_size = 0;
//                }
//            } else {
//                reg_UGPR_size = 0;
//            }
//            //cout << "reg_GPR_size: " << reg_GPR_size << endl;
//            //cout << "reg_PRED_size: " << reg_PRED_size << endl;
//            //cout << "reg_UGPR_size: " << reg_UGPR_size << endl;
//            //cout << "reg_UPRED_size: " << reg_UPRED_size << endl;
//
//        } else {
//            // match src file and corresponding line
////            vector<string> src_file = getMatch("(\\t*)\\/\\/## File \"(.*)\", line ([0-9]*)(.*)", tempStr);     //补上 else 排除
////            if (!src_file.empty()) {
////                filePath = src_file[1];
////                fileLine = src_file[2];
////                cout << "   Source File    Name: " << filePath << "       Line: " << fileLine << endl;
////                FI->addSrcFile(filePath, fileLine);
////
////                continue;
////            }
//
//
//
//
//            // count each register          // 写进function的else
////            vector<string> reg_count = getMatch("(.*)\\/\\/ \\|\\s*#(.*)\\s+\\|\\s*#(.*)\\s+\\|\\s*#(.*)\\s+\\|\\s*#(.*)\\s+\\|", tempStr);
////            if (!reg_count.empty()) {
////                reg_GPR_size = regCount(reg_count[1]);
////                //cout << "reg_GPR_size: " << reg_GPR_size << endl;
////                reg_PRED_size = regCount(reg_count[2]);
////                //cout << "reg_PRED_size: " << reg_PRED_size << endl;
////                reg_UGPR_size = regCount(reg_count[3]);
////                //cout << "reg_UGPR_size: " << reg_UGPR_size << endl;
////                reg_UPRED_size = regCount(reg_count[4]);
////                //cout << "reg_UPRED_size: " << reg_UPRED_size << endl;
////
////                continue;
////            }
//        }
//    }
//    if (FI != nullptr) {
//        //vec_FuncInfos.push_back(*FI);
//        map_FuncInfos.insert(pair<string, FuncInfo>(FI->getFuncName(), *FI));
//    }
//}
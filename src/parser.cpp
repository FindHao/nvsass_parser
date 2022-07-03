#include "parser.h"
#include <sstream>
#include <cstdio>


map<string, FuncInfo> map_FuncInfos;

//The reference positions for the instructions in helpFunc.h
int FP32_END;
int FP64_END;
int INT_END;

void init(string &path) {
    map_FuncInfos = mapOffset(path);
    INT_END = find(AssembFunc.begin(), AssembFunc.end(), "INT_END") - AssembFunc.begin();
    FP32_END = find(AssembFunc.begin(), AssembFunc.end(), "FP32_END") - AssembFunc.begin();
    FP64_END = find(AssembFunc.begin(), AssembFunc.end(), "FP64_END") - AssembFunc.begin();
}

void regUsageParse(const string &str_REG, Register &reg, const string &funcName, int offset, const string &sass_code) {
    if (str_REG.empty()) {  // no reg_GPR used now
        reg.occupied_count = 0;
        vector<RegisterStatus> reg_s(reg.size, REG_UNUSED);
        reg.reg_status = reg_s;
        //cout << "GPR empty" << endl;
    } else {
        // get the number of registers used
        int index = str_REG.find(' ') - 1;
        reg.occupied_count = strtol(str_REG.substr(0, index + 1).c_str(), nullptr, 10);
        //cout << "GPR occupied_count: " << reg.occupied_count << endl;
        for (int i = 0; i < reg.size; i++) {
            index = index + 1 + to_string(i).size();
            //cout << str_GPR[index] << endl;
            switch (str_REG[index]) {
                case ' ':
                    reg.reg_status.push_back(REG_UNUSED);
                    break;
                case '^':
                    reg.reg_status.push_back(REG_STORE);
                    break;
                case 'v':
                    reg.reg_status.push_back(REG_LOAD);
                    break;
                case 'x':
                    reg.reg_status.push_back(REG_LOAD_STORE);
                    break;
                case ':':
                    reg.reg_status.push_back(REG_IN_USE);
                    break;
                default:
                    fprintf(stderr, "ERROR: Unknown register status %c\n at FuncName: %s, offset: %d, Sass Code: %s\n",
                            str_REG[index], funcName.c_str(), offset, sass_code.c_str());
                    exit(0);
            }
        }
    }

}

map<string, FuncInfo> mapOffset(string &dataPath) {

    string tempStr;
    ifstream myfile;
    // Buffer
    char buf[64 * 1024];
    myfile.rdbuf()->pubsetbuf(buf, sizeof buf);
    myfile.open(dataPath);
    if (!myfile) {
        cout << "Unable to open myfile";
        exit(1); // terminate with error
    }
    FILE *fp;
    fp = fopen(dataPath.c_str(), "r");
    // Analyze line by line
    FuncInfo *funcinfo = nullptr;

    string filePath = "no src file";
    string fileLine = "-1";
    int reg_GPR_size = -1;
    int reg_PRED_size = -1;
    int reg_UGPR_size = -1;
    int reg_UPRED_size = -1;
    while (!feof(fp)) {
        fgets(buf, sizeof buf, fp);
        tempStr = buf;

        if (tempStr.empty())
            continue;
        tempStr.erase(0, tempStr.find_first_not_of(" \t"));
        if (tempStr[0] == '.')
            continue;
        if (tempStr[0] == '/' && tempStr[1] == '*') {
            // sass code line
            funcinfo->addSrcFile(filePath);
            int offset = hexToInt(tempStr.substr(2, tempStr.find("*/") - 2));
            //@Yueming Hao TODO: get rid of all the erase()
            // earse everything before assembly code
            tempStr.erase(0, tempStr.find_first_of(' '));
            tempStr.erase(0, tempStr.find_first_not_of(' '));
            string code = tempStr.substr(0, tempStr.find_first_of(';'));
            // erase everything before register part
            tempStr.erase(0, tempStr.find_first_of("//"));
            tempStr.erase(0, tempStr.find_first_of('|') + 1);
            tempStr.erase(0, tempStr.find_first_not_of(' '));
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

            string str_GPR = tempStr.substr(0, tempStr.find_first_of('|'));
            tempStr.erase(0, tempStr.find_first_of('|') + 1);
            tempStr.erase(0, tempStr.find_first_not_of(' '));

            string str_PRED = tempStr.substr(0, tempStr.find_first_of('|'));
            tempStr.erase(0, tempStr.find_first_of('|') + 1);
            tempStr.erase(0, tempStr.find_first_not_of(' '));

            string str_UGPR;
            string str_UPRED;
            int tempPos = tempStr.find_first_of('|');
            if (tempPos != tempStr.npos) {
                str_UGPR = tempStr.substr(0, tempPos);
                tempStr.erase(0, tempPos + 1);
                tempStr.erase(0, tempStr.find_first_not_of(' '));

                tempPos = tempStr.find_first_of('|');
                if (tempPos != tempStr.npos) {
                    str_UPRED = tempStr.substr(0, tempStr.find_first_of('|'));
                }
            }
            regUsageParse(str_GPR, *reg_GPR, funcinfo->getFuncName(), offset, code);
            regUsageParse(str_PRED, *reg_PRED, funcinfo->getFuncName(), offset, code);
            regUsageParse(str_UGPR, *reg_UGPR, funcinfo->getFuncName(), offset, code);
            regUsageParse(str_UPRED, *reg_UPRED, funcinfo->getFuncName(), offset, code);
            funcinfo->addOffsetSrc(offset, filePath, fileLine, code, reg_GPR, reg_PRED, reg_UGPR, reg_UPRED);
        } else if (tempStr[2] == '-') {
//            mangled kernel name
            if (funcinfo != nullptr) {
                if (map_FuncInfos.find(funcinfo->getFuncName()) != map_FuncInfos.end())
                    cout << "ERROR: Kernel Exists!" << endl;
                map_FuncInfos.insert(pair<string, FuncInfo>(funcinfo->getFuncName(), *funcinfo));
                //cout << "test: " << map_FuncInfos[funcinfo->getFuncName()].getFuncName() << endl;
            }
            tempStr.erase(0, tempStr.find_first_of('.'));
            string kernel_str = tempStr.substr(6, tempStr.find_first_of(" ---") - 6);
            auto *tempObj = new FuncInfo(kernel_str);
            funcinfo = tempObj;
            filePath = "no src file";
            fileLine = "-1";
            reg_GPR_size = -1;
            reg_PRED_size = -1;
            reg_UGPR_size = -1;
            reg_UPRED_size = -1;
            //cout << "Create: " << funcinfo->getFuncName() << endl;
        } else if (tempStr[1] == '/' && tempStr[2] == '#') {
            // source code file and line number
            tempStr.erase(0, tempStr.find_first_of('\"') + 1);
            filePath = tempStr.substr(0, tempStr.find_first_of('\"'));
            tempStr.erase(0, tempStr.find(", line") + 6);
            tempStr.erase(0, tempStr.find_first_not_of(' '));
            fileLine = tempStr.substr(0, tempStr.find_first_of(' '));
//            cout << "   Source File    Name: " << filePath << "       Line: " << fileLine << endl;
            funcinfo->addSrcFile(filePath);
        } else if (tempStr.find('#') != tempStr.npos && tempStr.find("// |") != tempStr.npos) {
            // count each register
            tempStr.erase(0, tempStr.find_first_of('#') + 1);
            reg_GPR_size = regCount(tempStr.substr(0, tempStr.find_first_of('|')));
            tempStr.erase(0, tempStr.find_first_of('#') + 1);
            reg_PRED_size = regCount(tempStr.substr(0, tempStr.find_first_of('|')));
            int tempPos = tempStr.find_first_of('#');
            if (tempPos != tempStr.npos) {
                tempStr.erase(0, tempPos + 1);
                reg_UGPR_size = regCount(tempStr.substr(0, tempStr.find_first_of('|')));
                tempPos = tempStr.find_first_of('#');
                if (tempPos != tempStr.npos) {
                    tempStr.erase(0, tempStr.find_first_of('#') + 1);
                    reg_UPRED_size = regCount(tempStr.substr(0, tempStr.find_first_of('|')));
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

        }

    }
    if (funcinfo != nullptr) {
        map_FuncInfos.insert(pair<string, FuncInfo>(funcinfo->getFuncName(), *funcinfo));
    }
    myfile.close();
    fclose(fp);
    return map_FuncInfos;
}

void inline fill_reg(kernel::mapRes::FuncInfo::SASSLineInfo::Register & reg, struct SASSLineInfo& sassline){
    reg.set_name(sassline.reg_GPR->name);
    reg.set_size(sassline.reg_GPR->size);
    reg.set_occupied_count(sassline.reg_GPR->occupied_count);
    for (int i = 0; i < sassline.reg_GPR->reg_status.size(); i++) {
        reg.add_reg_status(sassline.reg_GPR->reg_status[i]);
    }
}

string encode(map<string, FuncInfo> map_FI) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    kernel::mapRes maps;
    auto resMap = maps.mutable_testmap();
    for (auto iter = map_FI.begin(); iter != map_FI.end(); iter++) {
        // * Construct a FuncInfo object
        kernel::mapRes::FuncInfo *funcinfo = maps.add_fi();
        funcinfo->set_funcname(iter->second.getFuncName());
        auto set_srcfile = iter->second.getSrcFile();
        for (auto it_src = set_srcfile.begin(); it_src != set_srcfile.end(); it_src++) {
            funcinfo->add_srcfileset(*it_src);
        }
        // map_offset_sassline
        auto map_offset_src = funcinfo->mutable_map_offset_src();
        map<int, struct SASSLineInfo> temp_map = iter->second.getOffsetSrc();
        for (auto it_offset = temp_map.begin(); it_offset != temp_map.end(); it_offset++) {
            int offset = it_offset->first;
            // construct SASSLineInfo
            kernel::mapRes::FuncInfo::SASSLineInfo *SASSLine_1 = funcinfo->add_sassline();
            SASSLine_1->set_src_path(*it_offset->second.src_path);
            SASSLine_1->set_src_line(it_offset->second.src_line);
            SASSLine_1->set_code(*it_offset->second.code);
            // construct Register
            auto *reg_GPR = new kernel::mapRes::FuncInfo::SASSLineInfo::Register();
            auto *reg_PRED = new kernel::mapRes::FuncInfo::SASSLineInfo::Register();
            auto *reg_UGPR = new kernel::mapRes::FuncInfo::SASSLineInfo::Register();
            auto *reg_UPRED = new kernel::mapRes::FuncInfo::SASSLineInfo::Register();

            fill_reg(*reg_GPR, it_offset->second);
            fill_reg(*reg_PRED, it_offset->second);
            fill_reg(*reg_UGPR, it_offset->second);
            fill_reg(*reg_UPRED, it_offset->second);
            //set SASSLineInfo's reg status
            *SASSLine_1->mutable_reg_gpr() = *reg_GPR;
            *SASSLine_1->mutable_reg_pred() = *reg_PRED;
            *SASSLine_1->mutable_reg_ugpr() = *reg_UGPR;
            *SASSLine_1->mutable_reg_upred() = *reg_UPRED;
            // * Push SASSLineInfo into FuncInfo offset map
            (*map_offset_src)[offset] = *SASSLine_1;
        }
        // Push FuncInfo into maps
        (*resMap)[funcinfo->funcname()] = *funcinfo;
    }
    string serializedStr;
    maps.SerializeToString(&serializedStr);
    //cout<<"serialization result: "<<serializedStr<<endl;
    //cout<<endl<<"debugString:"<<maps.DebugString();
    return serializedStr;
}

//@TODO: do we need to change Map to std::map?
google::protobuf::Map<string, kernel::mapRes_FuncInfo> decode(string &serializedStr) {
    kernel::mapRes deserializedMapRes;
    if (!deserializedMapRes.ParseFromString(serializedStr)) {
        cerr << "Failed to parse maps." << endl;
    }
    //cout<<"deserializedStudent debugString:"<<deserializedMapRes.DebugString();
    cout << endl << "Map Results Size: " << deserializedMapRes.testmap().size() << endl;
    return deserializedMapRes.testmap();
}

ArrayType getArrayType(string& search_str){
    ArrayType array_type{};
    auto iter = std::find(AssembFunc.begin(), AssembFunc.end(), search_str);
    if (iter != AssembFunc.end()) {
        int index = distance(AssembFunc.begin(), iter);     // iter - begin
        if (index > FP32_END) {
            array_type.data_type = DATA_FLOAT;
            array_type.unit_size = 8;
        } else if (index < FP32_END && index >= INT_END) {
            array_type.data_type = DATA_FLOAT;
            array_type.unit_size = 4;
        } else {
//            @TODO: chceck modifers
            array_type.data_type = DATA_INT;
            array_type.unit_size = 4;
        }
    } else {
        cout << "ERROR: Not found this function in the set." << endl;
        exit(-1);
    }
    return array_type;
}

void searchOffset_protobuf(const kernel::mapRes::FuncInfo &funcinfo, int search_offset) {
    ArrayType resType{};
    kernel::mapRes::FuncInfo::SASSLineInfo sassline;
    auto iter = funcinfo.map_offset_src().find(search_offset);
    if (iter == funcinfo.map_offset_src().end()) {
        cout << "ERROR: Not exists this offset" << endl;
        exit(0);
    }
    sassline = iter->second;

    kernel::mapRes::FuncInfo::SASSLineInfo::Register reg_GPR = sassline.reg_gpr();
    string code = sassline.code();
    //cout << code << endl << endl;
    vector<string> vec_code = splitCode(code);
    //// split by '.',  get the "LDG" from "LDG.E.64"
    string funcType = splitFuncType(vec_code[0]);
    if (funcType == "LDG" || funcType == "LD") {
        cout << endl << "Load：" << endl;
        // find which reg is used (writen). example: "LDG.E.64 R8, [R16.64] ;" 中的 "R8"
        string get_reg = vec_code[1];
        int reg_write = atoi(get_reg.substr(1).c_str());
        if (reg_write < 0 || reg_write >= reg_GPR.size()) {
            cout << "ERROR: Index of LDG wrong." << endl;
        }

        // find where this reg is used (read)
        bool found = false;
        for (search_offset += 0x10;
             search_offset <= (funcinfo.map_offset_src().size() - 1) * 0x10; search_offset += 0x10) {
            auto iter1 = funcinfo.map_offset_src().find(search_offset);
            if (iter1 == funcinfo.map_offset_src().end()) {
                cout << "ERROR: Not exists this offset" << endl;
                exit(0);
            }
            sassline = iter1->second;
            reg_GPR = sassline.reg_gpr();
            auto cur_reg_status =  reg_GPR.reg_status()[reg_write];
            if ( cur_reg_status & REG_LOAD || cur_reg_status & REG_LOAD_STORE) {
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "ERROR: Not found where the register just written is read after loading process." << endl;
        }
        code = sassline.code();
        vec_code = splitCode(code);
        string tmp = splitFuncType(vec_code[0]);
        resType = getArrayType(tmp);
    } else if (funcType == "STG" || funcType == "ST") {    // Store
        cout << endl << "Store：" << endl;
        // find which reg is used (read)
        string get_reg = vec_code[2];  // "STG.E.64 [R14.64], R8;" R8
        int reg_getData = atoi(get_reg.substr(1).c_str());
        if (reg_getData < 0 || reg_getData >= reg_GPR.size()) {
            cout << "ERROR: Index of STG wrong." << endl;
        }
        // find when this reg is written
        bool found = false;
        for (search_offset -= 0x10; search_offset >= 0x00; search_offset -= 0x10) { // 16 -> 0x10
            auto iter1 = funcinfo.map_offset_src().find(search_offset);
            if (iter1 == funcinfo.map_offset_src().end()) {
                cout << "ERROR: Not exists this offset" << endl;
                exit(0);
            }
            sassline = iter1->second;
            reg_GPR = sassline.reg_gpr();
            if (reg_GPR.reg_status()[reg_getData] & REG_STORE) { // 1 -> 写 ^, 3 -> 读+写 x
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "ERROR: Not found where the register just read is written before storing process." << endl;
        }
        code = sassline.code();
        vec_code = splitCode(code);
        string tmp = splitFuncType(vec_code[0]);
        resType = getArrayType(tmp);
    } else {
        cout << "Not LD/LDG/ST/STG" << endl;
    }
}






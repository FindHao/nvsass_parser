#include <iostream>
#include <string>
#include "mapRes.pb.h"
#include "lib.h"
#include "DataType.h"
#include <sstream>

using namespace std;

void decode(string serializedStr);
string encode(map<string, FuncInfo> map_FI);
void searchOffset_protobuf(kernel::mapRes::FuncInfo FI, int search_offset);

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

string encode(map<string, FuncInfo> map_FI) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    kernel::mapRes maps;

    auto resMap = maps.mutable_testmap();
    for (auto iter = map_FI.begin(); iter != map_FI.end(); iter++) {
        // * Construct a FuncInfo object
        kernel::mapRes::FuncInfo* FI_1 = maps.add_fi();
        FI_1->set_funcname(iter->second.getFuncName());

        // ** set
        auto set_srcfile = iter->second.getSrcFile();
        for (auto it_src = set_srcfile.begin(); it_src != set_srcfile.end(); it_src++) {
            FI_1->add_srcfileset(*it_src);
        }
//        auto set_code = iter->second.getCodeSet();
//        for (auto it_code = set_code.begin(); it_code != set_code.end(); it_code++) {
//            FI_1->add_codeset(*it_code);
//        }

        // map_offset_sassline
        auto map_offset_src = FI_1->mutable_map_offset_src();
        map<int, struct SASSLineInfo> temp_map = iter->second.getOffsetSrc();
        for (auto it_offset = temp_map.begin(); it_offset != temp_map.end(); it_offset++) {
            int offset = it_offset->first;
            // ** construct SASSLineInfo
            kernel::mapRes::FuncInfo::SASSLineInfo* SASSLine_1 = FI_1->add_sassline();
            SASSLine_1->set_src_path(*it_offset->second.src_path);
            SASSLine_1->set_src_line(it_offset->second.src_line);
            SASSLine_1->set_code(*it_offset->second.code);

            // *** construct Register
            kernel::mapRes::FuncInfo::SASSLineInfo::Register* reg_GPR = new kernel::mapRes::FuncInfo::SASSLineInfo::Register();
            kernel::mapRes::FuncInfo::SASSLineInfo::Register* reg_PRED = new kernel::mapRes::FuncInfo::SASSLineInfo::Register();
            kernel::mapRes::FuncInfo::SASSLineInfo::Register* reg_UGPR = new kernel::mapRes::FuncInfo::SASSLineInfo::Register();
            kernel::mapRes::FuncInfo::SASSLineInfo::Register* reg_UPRED = new kernel::mapRes::FuncInfo::SASSLineInfo::Register();

            reg_GPR->set_name(it_offset->second.reg_GPR->name);
            reg_GPR->set_size(it_offset->second.reg_GPR->size);
            reg_GPR->set_occupied_count(it_offset->second.reg_GPR->occupied_count);
            for (int i = 0; i < it_offset->second.reg_GPR->reg_status.size(); i++) {
                reg_GPR->add_reg_status(it_offset->second.reg_GPR->reg_status[i]);
            }

            reg_PRED->set_name(it_offset->second.reg_PRED->name);
            reg_PRED->set_size(it_offset->second.reg_PRED->size);
            reg_PRED->set_occupied_count(it_offset->second.reg_PRED->occupied_count);
            for (int i = 0; i < it_offset->second.reg_PRED->reg_status.size(); i++) {
                reg_PRED->add_reg_status(it_offset->second.reg_PRED->reg_status[i]);
            }

            reg_UGPR->set_name(it_offset->second.reg_UGPR->name);
            reg_UGPR->set_size(it_offset->second.reg_UGPR->size);
            reg_UGPR->set_occupied_count(it_offset->second.reg_UGPR->occupied_count);
            for (int i = 0; i < it_offset->second.reg_UGPR->reg_status.size(); i++) {
                reg_GPR->add_reg_status(it_offset->second.reg_UGPR->reg_status[i]);
            }

            reg_UPRED->set_name(it_offset->second.reg_UPRED->name);
            reg_UPRED->set_size(it_offset->second.reg_UPRED->size);
            reg_UPRED->set_occupied_count(it_offset->second.reg_UPRED->occupied_count);
            for (int i = 0; i < it_offset->second.reg_UPRED->reg_status.size(); i++) {
                reg_UPRED->add_reg_status(it_offset->second.reg_UPRED->reg_status[i]);
            }


            // ** set SASSLineInfo's reg status
            *SASSLine_1->mutable_reg_gpr() = *reg_GPR;
            *SASSLine_1->mutable_reg_pred() = *reg_PRED;
            *SASSLine_1->mutable_reg_ugpr() = *reg_UGPR;
            *SASSLine_1->mutable_reg_upred() = *reg_UPRED;

            // * Push SASSLineInfo into FuncInfo offset map
            (*map_offset_src)[offset] = *SASSLine_1;
        }



        // Push FuncInfo into maps
        (*resMap)[FI_1->funcname()] = *FI_1;
    }



    // 对消息对象student序列化到string容器
    string serializedStr;
    maps.SerializeToString(&serializedStr);
    //cout<<"serialization result: "<<serializedStr<<endl; //序列化后的字符串内容是二进制内容，非可打印字符，预计输出乱码
    //cout<<endl<<"debugString:"<<maps.DebugString();

    return serializedStr;
}

void decode(string serializedStr) {

    //解析序列化后的消息对象，即反序列化
    kernel::mapRes deserializedMapRes;
    if(!deserializedMapRes.ParseFromString(serializedStr)){
        cerr << "Failed to parse maps." << endl;
    }

    cout<<"-------------上面是序列化，下面是反序列化---------------"<<endl;
    //打印解析后的student消息对象
    //cout<<"deserializedStudent debugString:"<<deserializedMapRes.DebugString();
    cout <<endl<<"Map Results Size: " << deserializedMapRes.testmap().size() << endl;
    auto temp_map = deserializedMapRes.testmap();


    // transfer to map<string, FuncInfo>
    /*
    for (auto it_func = temp_map.begin(); it_func != temp_map.end(); it_func++) {
        string funcName = it_func->first;
        FuncInfo *tempFI = new FuncInfo(funcName);

        // set
        auto set_srcfile = it_func->second.srcfileset();
        for (auto it_src = set_srcfile.begin(); it_src != set_srcfile.end(); it_src++) {
            tempFI->addSrcFile(*it_src);
        }

        // SASSLine
        auto map_offset = it_func->second.map_offset_src();
        for (auto it_offset = map_offset.begin(); it_offset != map_offset.end(); it_offset++) {
            // void FuncInfo::addOffsetSrc(int offset, string filePath, string line, string code, Register *reg_GPR, Register *reg_PRED, Register *reg_UGPR, Register *reg_UPRED)
            int offset = it_offset->first;
            string filePath = it_offset->second.src_path();
            string line = to_string(it_offset->second.src_line());
            string code = it_offset->second.code();
            Register *reg_GPR = new Register();
            Register *reg_PRED = new Register();
            Register *reg_UGPR = new Register();
            Register *reg_UPRED = new Register();

            tempFI->addOffsetSrc(offset, filePath, line, code, reg_GPR, reg_PRED, reg_UGPR, reg_UPRED);
        }

        // push this FuncInfo to de_map
        de_map.insert(pair<string, FuncInfo>(funcName, *tempFI));
    }
    */


    // simple test
    /*
    auto it = deserializedMapRes.testmap().find("__cuda_sm20_div_f64_slowpath_v2");
    if (it != deserializedMapRes.testmap().end()) {
        cout << "Func Name: " << it->second.funcname() << endl;

        auto it_0 = it->second.map_offset_src().find(16);
        if (it_0 != it->second.map_offset_src().end()) {
            cout << it_0->second.src_path() << " line: " << (int)it_0->second.src_line() << endl;
            cout << it_0->second.code() << endl;
            cout << it_0->second.reg_gpr().name() << " " << it_0->second.reg_gpr().reg_status()[1] << endl;
            cout << it_0->second.reg_gpr().name() << " " << it_0->second.reg_gpr().size() << endl;
        }
    }
    */

    searchOffset_protobuf(temp_map["_ZN3cub11EmptyKernelIvEEvv"], 0);
    searchOffset_protobuf(temp_map["_ZN5amrex13launch_globalILi256EZNS_11ParallelForIiZNS_7BaseFabIiE6setValILNS_5RunOnE1EEEvRKiRKNS_3BoxENS_8DestCompENS_8NumCompsEEUliiiiE_vEENSt9enable_ifIXsr5amrex19MaybeDeviceRunnableIT0_vEE5valueEvE4typeERKNS_3Gpu10KernelInfoESA_T_OSF_EUlvE_EEvSF_"], 64);
    searchOffset_protobuf(temp_map["_ZN5amrex13launch_globalILi256EZNS_11ParallelForIiZNS_7BaseFabIiE6setValILNS_5RunOnE1EEEvRKiRKNS_3BoxENS_8DestCompENS_8NumCompsEEUliiiiE_vEENSt9enable_ifIXsr5amrex19MaybeDeviceRunnableIT0_vEE5valueEvE4typeERKNS_3Gpu10KernelInfoESA_T_OSF_EUlvE_EEvSF_"], 144);
    searchOffset_protobuf(temp_map["_ZN5amrex13launch_globalILi256EZNS_11ParallelForIiZNS_7BaseFabIiE6setValILNS_5RunOnE1EEEvRKiRKNS_3BoxENS_8DestCompENS_8NumCompsEEUliiiiE_vEENSt9enable_ifIXsr5amrex19MaybeDeviceRunnableIT0_vEE5valueEvE4typeERKNS_3Gpu10KernelInfoESA_T_OSF_EUlvE_EEvSF_"], 240);


    google::protobuf::ShutdownProtobufLibrary();

}

// 参数 offset, kernel name
void searchOffset_protobuf(kernel::mapRes::FuncInfo FI, int search_offset) {
    DataType resType;
    kernel::mapRes::FuncInfo::SASSLineInfo SI;
    auto iter = FI.map_offset_src().find(search_offset);
    if (iter == FI.map_offset_src().end()) {
        cout << "ERROR: Not exists this offset" << endl;
        exit(0);
    }
    SI = iter->second;

    kernel::mapRes::FuncInfo::SASSLineInfo::Register reg_GPR = SI.reg_gpr();
    string code = SI.code();
    //cout << code << endl << endl;
    vector<string> vec_code = splitCode(code);

    string funcType = splitFuncType(vec_code[0]);   // split by '.',  get the "LDG" from "LDG.E.64"
    if (funcType == "LDG" || funcType == "LD") {   // Load         // 改成 "LDG" 在不在 string 里    // 已知 offset       // store  "ST"
        cout << endl << "Load：" << endl;

        // find which reg is used (writen)
        string get_reg = vec_code[1]; // "LDG.E.64 R8, [R16.64] ;" 中的 "R8"
        int reg_write = atoi(get_reg.substr(1).c_str());
        if (reg_write < 0 || reg_write >= reg_GPR.size()) {
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
        for (search_offset += 0x10; search_offset <= (FI.map_offset_src().size() - 1) * 0x10; search_offset += 0x10) { // 16 -> 0x10
            auto iter1 = FI.map_offset_src().find(search_offset);
            if (iter1 == FI.map_offset_src().end()) {
                cout << "ERROR: Not exists this offset" << endl;
                exit(0);
            }
            SI = iter1->second;

            reg_GPR = SI.reg_gpr();
            if ((reg_GPR.reg_status()[reg_write] & 0x2) == 0x2) { // 2 -> 读 v, 3 -> 读+写 x     // & 0x2
                found = true;
                break;  // find the read line
            }
        }

        // if 没找到  error
        if (!found) {
            cout << "ERROR: Not found where the register just written is read after loading process." << endl;
        }

        code = SI.code();
        vec_code = splitCode(code);


        // enum / map
        auto iter = std::find(AssembFunc.begin(), AssembFunc.end(), splitFuncType(vec_code[0]));
        if (iter != AssembFunc.end()) {
            int index = distance(AssembFunc.begin(), iter);     // iter - begin
            cout << "index: " << index << endl;
            if (index >= 31) {
                cout << "Type is FP64" << endl;        // 类型建structure  1. char type  0-> int  1-> float  2. 几bit的类型 32-> FP32  16-> FP64      返回这个结构体
            } else if (index < 31 && index >= 21) {
                cout << "Type is FP32" << endl;
            } else { // index < 21
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
        if (reg_getData < 0 || reg_getData >= reg_GPR.size()) {
            cout << "ERROR: Index of STG wrong." << endl;
        }

        // find when this reg is written
        bool found = false;
        for (search_offset -= 0x10; search_offset >= 0x00; search_offset -= 0x10) { // 16 -> 0x10
            auto iter1 = FI.map_offset_src().find(search_offset);
            if (iter1 == FI.map_offset_src().end()) {
                cout << "ERROR: Not exists this offset" << endl;
                exit(0);
            }
            SI = iter1->second;

            reg_GPR = SI.reg_gpr();
            if ((reg_GPR.reg_status()[reg_getData] & 0x1) == 0x1) { // 1 -> 写 ^, 3 -> 读+写 x
                found = true;
                break;  // find the read line
            }
        }

        // if 没找到  error
        if (!found) {
            cout << "ERROR: Not found where the register just read is written before storing process." << endl;
        }

        code = SI.code();
        vec_code = splitCode(code);

        auto iter = std::find(AssembFunc.begin(), AssembFunc.end(), splitFuncType(vec_code[0]));
        if (iter != AssembFunc.end()) {
            int index = distance(AssembFunc.begin(), iter);     // iter - begin
            cout << "index: " << index << endl;
            if (index >= 31) {
                cout << "Type is FP64" << endl;        // 类型建structure  1. char type  0-> int  1-> float  2. 几bit的类型 32-> FP32  16-> FP64      返回这个结构体
                resType.type = 1;
                resType.version = 64;
            } else if (index < 31 && index >= 21) {
                cout << "Type is FP32" << endl;
                resType.type = 1;
                resType.version = 32;
            } else { // index < 21
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

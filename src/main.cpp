#include <iostream>
#include <string>
#include "parser.h"
#include <sstream>

using namespace std;


int main(int argc, char *argv[]) {

    string path("../data/castro1.txt");
    parser_init(path);
    cout << "Test Name: " << map_FuncInfos.begin()->second.getFuncName() << endl;
//    cout << "__cuda_sm20_div_f64_slowpath_v2: " << map_FuncInfos["__cuda_sm20_div_f64_slowpath_v2"].searchOffset(0).src_line << endl;

    string serializedStr = encode(map_FuncInfos);

    ofstream OsWrite("store.txt");
    OsWrite << serializedStr;
    OsWrite.close();

    ifstream IsRead;
    IsRead.open("store.txt");

//    auto ss = std::ostringstream();
    ostringstream ss;
    ss << IsRead.rdbuf();
    string strToDecode = ss.str();

    auto temp_map = decode(strToDecode);
    searchOffset_protobuf(temp_map["_ZN3cub11EmptyKernelIvEEvv"], 0);
    searchOffset_protobuf(
            temp_map["_ZN5amrex13launch_globalILi256EZNS_11ParallelForIiZNS_7BaseFabIiE6setValILNS_5RunOnE1EEEvRKiRKNS_3BoxENS_8DestCompENS_8NumCompsEEUliiiiE_vEENSt9enable_ifIXsr5amrex19MaybeDeviceRunnableIT0_vEE5valueEvE4typeERKNS_3Gpu10KernelInfoESA_T_OSF_EUlvE_EEvSF_"],
            64);
    searchOffset_protobuf(
            temp_map["_ZN5amrex13launch_globalILi256EZNS_11ParallelForIiZNS_7BaseFabIiE6setValILNS_5RunOnE1EEEvRKiRKNS_3BoxENS_8DestCompENS_8NumCompsEEUliiiiE_vEENSt9enable_ifIXsr5amrex19MaybeDeviceRunnableIT0_vEE5valueEvE4typeERKNS_3Gpu10KernelInfoESA_T_OSF_EUlvE_EEvSF_"],
            144);
    searchOffset_protobuf(
            temp_map["_ZN5amrex13launch_globalILi256EZNS_11ParallelForIiZNS_7BaseFabIiE6setValILNS_5RunOnE1EEEvRKiRKNS_3BoxENS_8DestCompENS_8NumCompsEEUliiiiE_vEENSt9enable_ifIXsr5amrex19MaybeDeviceRunnableIT0_vEE5valueEvE4typeERKNS_3Gpu10KernelInfoESA_T_OSF_EUlvE_EEvSF_"],
            240);
    
    
    //  Transfer google::protobuf::Map to std::map
//     std::map<string, kernel::mapRes_FuncInfo> std_map(temp_map.begin(), temp_map.end());
//     // castro1.txt
//     searchOffset_protobuf(std_map["_ZN3cub11EmptyKernelIvEEvv"], 0);
//     searchOffset_protobuf(std_map["_ZN5amrex13launch_globalILi256EZNS_11ParallelForIiZNS_7BaseFabIiE6setValILNS_5RunOnE1EEEvRKiRKNS_3BoxENS_8DestCompENS_8NumCompsEEUliiiiE_vEENSt9enable_ifIXsr5amrex19MaybeDeviceRunnableIT0_vEE5valueEvE4typeERKNS_3Gpu10KernelInfoESA_T_OSF_EUlvE_EEvSF_"], 64);
//     searchOffset_protobuf(std_map["_ZN5amrex13launch_globalILi256EZNS_11ParallelForIiZNS_7BaseFabIiE6setValILNS_5RunOnE1EEEvRKiRKNS_3BoxENS_8DestCompENS_8NumCompsEEUliiiiE_vEENSt9enable_ifIXsr5amrex19MaybeDeviceRunnableIT0_vEE5valueEvE4typeERKNS_3Gpu10KernelInfoESA_T_OSF_EUlvE_EEvSF_"], 144);
//     searchOffset_protobuf(std_map["_ZN5amrex13launch_globalILi256EZNS_11ParallelForIiZNS_7BaseFabIiE6setValILNS_5RunOnE1EEEvRKiRKNS_3BoxENS_8DestCompENS_8NumCompsEEUliiiiE_vEENSt9enable_ifIXsr5amrex19MaybeDeviceRunnableIT0_vEE5valueEvE4typeERKNS_3Gpu10KernelInfoESA_T_OSF_EUlvE_EEvSF_"], 240);

    
    return 0;
}

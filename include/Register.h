#ifndef NVSASS_PARSER_REGISTER_H
#define NVSASS_PARSER_REGISTER_H

#include <string>
#include <vector>

using std::string;
using std::vector;

enum RegisterStatus {
    REG_UNUSED = 0,
    REG_STORE,
    REG_LOAD,
    REG_LOAD_STORE,
    REG_IN_USE,
};
struct Register {
    string name;
    int size;
    int occupied_count;
    vector<RegisterStatus> reg_status;
};

#endif //NVSASS_PARSER_REGISTER_H

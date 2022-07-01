#ifndef NVSASS_PARSER_REGISTER_H
#define NVSASS_PARSER_REGISTER_H

#include <string>
#include <map>

using namespace std;

struct Register {
    string name;
    int size;
    int occupied_count;
    vector<int> reg_status;     // 0    <space> : Register not in use
                                // 1    ^       : Register assignment                   写
                                // 2    v       : Register usage                        读
                                // 3    x       : Register usagde and reassignment       写+读
                                // 4    :       : Register in use


};

#endif //NVSASS_PARSER_REGISTER_H

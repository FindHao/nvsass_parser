#ifndef NVSASS_PARSER_DATATYPE_H
#define NVSASS_PARSER_DATATYPE_H

#include <string>
#include <map>

using namespace std;

struct DataType {
    int type;       // 0 -> INT, 1 -> FP
    int version;    // 32, 64
};


#endif //NVSASS_PARSER_DATATYPE_H

#ifndef NVSASS_PARSER_ARRAYTYPE_H
#define NVSASS_PARSER_ARRAYTYPE_H

#include <string>
#include <map>

using namespace std;

typedef enum DataType {
    DATA_UNKNOWN = 0,
    DATA_FLOAT = 1,
    DATA_INT = 2
} data_type_t;

struct ArrayType {
    data_type_t data_type;
    int32_t unit_size;
};


#endif //NVSASS_PARSER_ARRAYTYPE_H

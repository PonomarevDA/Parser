#ifndef OBD
#define OBD

#include "stdafx.h"
#include "data_structures.hpp"

enum optcode_t
{
    OPTCODE_ADD         = 0x18,
    OPTCODE_SUB         = 0x19,
    OPTCODE_MUL         = 0x1A,
    OPTCODE_DIV         = 0x1B,
    OPTCODE_AND         = 0x0D,
    OPTCODE_OR          = 0x0F,
    OPTCODE_LEFT_SHIFT  = 0x16,
    OPTCODE_RIGHT_SHIFT = 0x17,
};

struct ParamTable_t
{
    uint8_t flength;
    uint8_t Formula[32];
    Tree TreeParam();
};


void Do();
void Show_formula();

#endif // OBD

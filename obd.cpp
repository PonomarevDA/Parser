#include "stdafx.h"
#include "obd.hpp"
#include "cstring"  // для memcpy
#include <iostream>

static Stack buf;
static ParamTable_t ParamTable[1];
static uint8_t ParamCount = 1;

void Do()
{
    ParamTable[0].flength = 32;
    uint8_t arr[32] = {0x01, 0x1B, 0x1B, 0x18, 0x1A, 0x18, 0x1A, 0x19,
                       0x80, 0x82, 0x17, 0x03, 0x87, 0x81, 0x0D, 0x16,
                       0x0F, 0x03, 0xFF, 0x88, 0x02, 0x1A, 0x1A, 0x1A,
                       0x17, 0x03, 0x87, 0xA0, 0xA0, 0xA0, 0xA8, 0x99};
    memcpy(ParamTable[0].Formula, arr, ParamTable[0].flength);
}

void Show_formula()
{
    std::cout << "Formula[0]:" << std::endl;
    std::cout << ParamTable[0].flength + 0 << std::endl;
    for(uint8_t count = 1; count < ParamTable[0].flength; count++)
    {
        uint8_t byte = ParamTable[0].Formula[count];
        if(byte < 0x08)
            std::cout << "d" << byte + 0 << " ";
        else if(byte >= 0x80)
            std::cout << byte - 0x80 << " ";
        else if(byte == OPTCODE_ADD)
            std::cout << "+ ";
        else if(byte == OPTCODE_SUB)
            std::cout << "- ";
        else if(byte == OPTCODE_DIV)
            std::cout << "/ ";
        else if(byte == OPTCODE_MUL)
            std::cout << "* ";
        else if(byte == OPTCODE_AND)
            std::cout << "& ";
        else if(byte == OPTCODE_OR)
            std::cout << "| ";
        else if(byte == OPTCODE_LEFT_SHIFT)
            std::cout << "<< ";
        else if(byte == OPTCODE_RIGHT_SHIFT)
            std::cout << "<< ";
        else
            std::cout << "? ";
    }
}

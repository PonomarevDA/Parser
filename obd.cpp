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
    const uint8_t paramNumber = 0;
    uint8_t arr[32] = {0x01, 0x1B, 0x1B, 0x18, 0x1A, 0x18, 0x1A, 0x19,
                       0x80, 0x82, 0x17, 0x03, 0x87, 0x81, 0x0D, 0x16,
                       0x0F, 0x03, 0xFF, 0x88, 0x02, 0x1A, 0x1A, 0x1A,
                       0x17, 0x03, 0x87, 0xA0, 0xA0, 0xA0, 0xA8, 0x99};
    memcpy(ParamTable[paramNumber].formula, arr, ParamTable[paramNumber].flength);
    Parser();
}

void Parser()
{
    const uint8_t paramNumber = 0;

    for(uint8_t count = ParamTable[paramNumber].flength; count >= 1;)
    {
        uint8_t byte = ParamTable[paramNumber].formula[--count];
        // Если операнд
        if( (byte < 0x08) || (byte >= 0x80) )
        {
            Tree::Node* op = ParamTable[paramNumber].tree.add_node_lower(byte);
            buf.push(op);
        }
        // Если оператор
        else
        {
            uint8_t length = 2;
            Tree::Node** ops = new Tree::Node*[length];
            ops[0] = buf.pop();
            ops[1] = buf.pop();
            Tree::Node* op = ParamTable[paramNumber].tree.add_node_parent(byte, ops, length);
            buf.push(op);
        }
    }
}

void Show_formula()
{
    const uint8_t paramNumber = 0;
    std::cout << "Formula[0]:" << std::endl;
    std::cout << ParamTable[paramNumber].flength + 0 << std::endl;
    for(uint8_t count = 1; count < ParamTable[paramNumber].flength; count++)
    {
        uint8_t byte = ParamTable[paramNumber].formula[count];
        Show_byte(byte);
    }
}

void Show_byte(uint8_t byte)
{
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

void Show_tree()
{
    const uint8_t paramNumber = 0;
    Tree::Node* ptrNode = ParamTable[paramNumber].tree.get_base_node();
    std::cout << std::endl << "Tree:" << std::endl;
    if (ptrNode != nullptr)
    {
        Show_operator(ptrNode);
    }
    else
        std::cout <<std::endl << "Tree is empty";
}


void Show_operator(Tree::Node* ptrNode)
{
    if (ptrNode != nullptr)
    {
        if ( (ptrNode->Value >= 0x08) && (ptrNode->Value < 0x80) )
        {
            Show_byte(ptrNode->Value);
            Show_operator(ptrNode->ChildsArr[0]);
            Show_operator(ptrNode->ChildsArr[1]);
        }
        else
            std::cout << std::endl;
    }
}

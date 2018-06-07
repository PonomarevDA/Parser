#ifndef __OBD
#define __OBD

#include "data_structures.hpp"

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned int uint32_t;
typedef signed long long int64_t;

enum Opcode_t
{
	OPCODE_LOG_OR = 0x09, ///< ||
	OPCODE_LOG_AND = 0x0A, ///< &&
	OPCODE_LOG_NOT = 0x0B, ///< !
	OPCODE_BIT_NOT = 0x0C, ///< ~
	OPCODE_BIT_OR = 0x0D, ///< |
	OPCODE_BIT_XOR = 0x0E, ///< ^
	OPCODE_BIT_AND = 0x0F, ///< &
	OPCODE_EQU = 0x10, ///< ==
	OPCODE_NEQU = 0x11, ///< <>
	OPCODE_LESS = 0x12, ///< <
	OPCODE_MORE = 0x13, ///< >
	OPCODE_LESS_EQU = 0x14, ///< <=
	OPCODE_MORE_EQU = 0x15, ///< >=
	OPCODE_SHIFT_LEFT = 0x16, ///< <<
	OPCODE_SHIFT_RIGHT = 0x17, ///< >>
	OPCODE_ADD = 0x18, ///< +
	OPCODE_SUB = 0x19, ///< -
	OPCODE_MUL = 0x1A, ///< *
	OPCODE_DIV = 0x1B, ///< /
	OPCODE_IF_ELSE = 0x1C,	///< ? IF-ELSE
};

struct Frame
{
	Frame(uint8_t d0 = 0, uint8_t d1 = 0, uint8_t d2 = 0, uint8_t d3 = 0, uint8_t d4 = 0, uint8_t d5 = 0, uint8_t d6 = 0, uint8_t d7 = 0)
	{
		Data[0] = d0; Data[1] = d1; Data[2] = d2; Data[3] = d3; Data[4] = d4; Data[5] = d5; Data[6] = d6; Data[7] = d7;
	}
	uint8_t Data[8];
};

struct ParamTable_t
{
    ParamTable_t(): FormulaLength(0), tree(32) {}
    uint8_t FormulaLength;
    uint8_t Formula[32];
    Tree tree;
};

void method_dichotomy(int64_t NeedValue);
void brute_force(int64_t NeedValue);
void Test_calculation();
uint32_t Calculate(uint8_t opcode, uint32_t operand1, uint32_t operand2 = 0, uint32_t operand3 = 0);
void Show_operator(Tree::Node*);
void Show_tree();
void Show_byte(uint8_t byte);
void Show_formula();
void Parser();
void Init();
void Do();


#endif // __OBD

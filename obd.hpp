#ifndef __OBD
#define __OBD

#include "data_structures.hpp"

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned int uint32_t;
typedef signed long long int64_t;

class OBD
{
public:
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
		uint8_t DataBytes[4];
    };
    void Init();						// Инициализация протокола: построение дерева
    void Do();
protected:
	enum
	{
		MAX_NUMBER_OF_DATA_BYTES = 4,
	};
    // Метод инициализации:
    void CreateTrees();					// Построить дерево синтаксического разбора

    // Методы выполнения алгоритма прямого/обратного расчета:
    void DoDirectCalculate();
    uint32_t DoReverseCalculateWithTree(uint32_t value, Tree::Node* node = nullptr);
    uint8_t DoReverseCalculateWithMethodDichotomy(int64_t NeedValue);
    uint8_t DoReverseCalculateWithBruteForce(int64_t NeedValue);
    uint32_t CalculateDirectElementary(uint8_t opcode, uint32_t operand1, uint32_t operand2 = 0, uint32_t operand3 = 0);
    uint32_t CalculateReverseElementary(uint32_t value, uint8_t opcode, uint32_t operand1, uint32_t operand2 = 0, uint32_t operand3 = 0);

    // Методы иллюстрации результатов алгоритмов в терминале:
    void ShowFormula();
    void ShowByte(uint8_t byte);
    void ShowTree();
    void ShowTreeNode(Tree::Node*);

    // Global and static variables:
    Stack buf;
    ParamTable_t ParamTable[9];
    uint8_t ParamNumber;
	uint8_t ParamCount;
    Frame frame;
    uint32_t Value = 0;
};
#endif // __OBD

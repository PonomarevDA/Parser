#ifndef __OBD
#define __OBD

#include "data_structures.hpp"

// Appendix [should use types.hpp]
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned int uint32_t;
typedef signed long long int64_t;

/// Declarations
class Calculator;

/// Operator codes: it need for OBD(CreateTree) and Calculator (most methods)
enum Opcode_t
{
	OPCODE_LOG_OR = 0x09,		///< ||
	OPCODE_LOG_AND = 0x0A,		///< &&
	OPCODE_LOG_NOT = 0x0B,		///< !
	OPCODE_BIT_NOT = 0x0C,		///< ~
	OPCODE_BIT_OR = 0x0D,		///< |
	OPCODE_BIT_XOR = 0x0E,		///< ^
	OPCODE_BIT_AND = 0x0F,		///< &
	OPCODE_EQU = 0x10,			///< ==
	OPCODE_NEQU = 0x11,			///< <>
	OPCODE_LESS = 0x12,			///< <
	OPCODE_MORE = 0x13,			///< >
	OPCODE_LESS_EQU = 0x14,		///< <=
	OPCODE_MORE_EQU = 0x15,		///< >=
	OPCODE_SHIFT_LEFT = 0x16,	///< <<
	OPCODE_SHIFT_RIGHT = 0x17,	///< >>
	OPCODE_ADD = 0x18,			///< +
	OPCODE_SUB = 0x19,			///< -
	OPCODE_MUL = 0x1A,			///< *
	OPCODE_DIV = 0x1B,			///< /
	OPCODE_IF_ELSE = 0x1C,		///< ? IF-ELSE
};

/*
* @brief Протокол OBD
*/
class OBD
{
public:
    /// Table of parameters, that storing information about sensors from CanBase
    struct ParamTable_t
    {
        ParamTable_t(): FormulaLength(0), tree(32) {}
        uint8_t FormulaLength;
        uint8_t Formula[32];
        Tree tree;
        uint8_t DataBytes[4];
    };
	
protected:
	friend Calculator;	/// because we need access to private data of this class
	/// Some constans
    enum
    {
        MAX_NUMBER_OF_DATA_BYTES = 4,
    };
    /// Method init:
    void CreateTrees();

	/// Methods for check the byte type
	friend uint8_t IsItDataFrame(const uint8_t byte);
	friend uint8_t IsItConst(const uint8_t byte);
	friend uint8_t IsItOperator(const uint8_t byte);
	friend uint8_t IsItOperand(const uint8_t byte);
	friend uint8_t IsItUnaryOperator(const uint8_t byte);
	friend uint8_t IsItBinaryOperator(const uint8_t byte);
	friend uint8_t IsItTernaryOperator(const uint8_t byte);

    /// Variables:
    ParamTable_t ParamTable[9];
    uint8_t ParamNumber;
};


/*
* @brief Калькулятор, решающий прямую и обратную задачи
*/
class Calculator
{
public:
	Calculator(uint32_t value, uint8_t paramNum, OBD& obd);
	uint32_t DoReverseCalculateWithTree(uint32_t value, const Tree::Node* node = nullptr);
	uint8_t DoReverseCalculateWithMethodDichotomy(const int64_t NeedValue);
	uint8_t DoReverseCalculateWithBruteForce(const int64_t NeedValue);
	void DoDirectCalculate();
	void PutValue(uint32_t value);
	uint8_t* GetDataFrame();
	uint32_t GetValue();
private:
	/// Variables
	uint32_t Value;
	Stack stack;
	uint8_t DataFrame[8];
	OBD::ParamTable_t* ptrParamTable;

	/// Elementary methods for do direct/reverse calculation algorithm
	uint32_t CalculateDirectElementary(const uint8_t opcode, const uint32_t operand1, const uint32_t operand2 = 0, const uint32_t operand3 = 0);
	uint32_t CalculateReverseElementary(uint32_t value, const uint8_t opcode, const uint32_t operand1, const uint32_t operand2 = 0, const uint32_t operand3 = 0);
};

#endif // __OBD

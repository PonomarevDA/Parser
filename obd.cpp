#include "obd.hpp"
#include "cstring"  // для memcpy
#include <iostream>

static Stack buf;
static ParamTable_t ParamTable[1];
static uint8_t ParamCount = 1;
static Frame frame;
static uint32_t Value = 0;

void Init()
{
    
    const uint8_t paramNumber = 0;
	/*
	ParamTable[0].flength = 32;
    uint8_t arr[32] = {0x01, 0x1B, 0x1B, 0x18, 0x1A, 0x18, 0x1A, 0x19,
                       0x80, 0x82, 0x17, 0x03, 0x87, 0x81, 0x0D, 0x16,
                       0x0F, 0x03, 0xFF, 0x88, 0x02, 0x1A, 0x1A, 0x1A,
                       0x17, 0x03, 0x87, 0xA0, 0xA0, 0xA0, 0xA8, 0x99};
    */
	ParamTable[0].FormulaLength = 12;
	uint8_t arr[12] = { 0x01, 0x1A, 0x0D, 0x0D, 0x16, 0x02, 0x90, 0x16, 0x01, 0x88, 0x00, 0x8A};
	memcpy(ParamTable[paramNumber].Formula, arr, ParamTable[paramNumber].FormulaLength);
    Parser();
}

void Do()
{
	uint8_t param = 0;
	uint8_t operators[16];
	uint32_t operands[16];
	int8_t operatorIndex = -1;
	int8_t operandIndex = -1;

	bool ignore = false;

	for (uint8_t index = ParamTable[param].FormulaLength - 1; index >= 1; index--)
	{
		uint8_t byte = ParamTable[param].Formula[index];

		/// Îïåðàíä (èíäåêñ ÿ÷åéêè êàäðà)
		if (byte < 8)
		{
			operands[++operandIndex] = frame.Data[byte];
		}
		/// Îïåðàòîð
		else if ((byte >= 8) && (byte <= 127))
		{
			operators[++operatorIndex] = byte;
		}
		/// Îïåðàíä (êîíñòàíòà)
		else
		{
			operands[++operandIndex] = (byte & 0x7F);
		}

		/// Ïðîâåðèòü, ìîæíî ëè âûïîëíèòü îïåðàöèþ
		if ((operatorIndex >= 0) && (operandIndex >= 1))
		{
			/// Âûáðàòü ÷èñëî îïåðàíäîâ, â çàâèñèìîñòè îò îïåðàòîðà
			if ((operators[operatorIndex] == OPCODE_LOG_NOT) || (operators[operatorIndex] == OPCODE_BIT_NOT))
			{
				uint32_t result = Calculate(operators[operatorIndex], operands[operandIndex]);
				operatorIndex -= 1;
				operands[operandIndex] = result;
			}
			else if ((operators[operatorIndex] == OPCODE_IF_ELSE) && (operandIndex == 2))
			{
				uint32_t result = Calculate(operators[operatorIndex], operands[operandIndex], operands[operandIndex - 1], operands[operandIndex - 2]);
				if (result == 0xFFFFFFFF)
				{
					ignore = true;
					break;
				}
				operatorIndex -= 1;
				operandIndex -= 2;
				operands[operandIndex] = result;
			}
			else
			{
				uint32_t result = Calculate(operators[operatorIndex], operands[operandIndex], operands[operandIndex - 1]);
				operatorIndex -= 1;
				operandIndex -= 1;
				operands[operandIndex] = result;
			}
		}
	}
	Value = operands[0];
}


uint32_t Calculate(uint8_t opcode, uint32_t operand1, uint32_t operand2, uint32_t operand3)
{
	if (opcode == OPCODE_LOG_OR)			return operand1 || operand2;
	else if (opcode == OPCODE_LOG_AND)		return operand1 && operand2;
	else if (opcode == OPCODE_LOG_NOT)		return !operand1;
	else if (opcode == OPCODE_BIT_NOT)		return ~operand1;
	else if (opcode == OPCODE_BIT_OR)		return operand1 | operand2;
	else if (opcode == OPCODE_BIT_XOR)		return operand1 ^ operand2;
	else if (opcode == OPCODE_BIT_AND)		return operand1 & operand2;
	else if (opcode == OPCODE_EQU)			return operand1 == operand2;
	else if (opcode == OPCODE_NEQU)			return operand1 != operand2;
	else if (opcode == OPCODE_LESS)			return operand1 < operand2;
	else if (opcode == OPCODE_MORE)			return operand1 > operand2;
	else if (opcode == OPCODE_LESS_EQU)		return operand1 <= operand2;
	else if (opcode == OPCODE_MORE_EQU)		return operand1 >= operand2;
	else if (opcode == OPCODE_SHIFT_LEFT)	return operand1 << operand2;
	else if (opcode == OPCODE_SHIFT_RIGHT)	return operand1 >> operand2;
	else if (opcode == OPCODE_ADD)			return operand1 + operand2;
	else if (opcode == OPCODE_SUB)			return operand1 - operand2;
	else if (opcode == OPCODE_MUL)			return operand1 * operand2;
	else if (opcode == OPCODE_DIV)			return operand1 / operand2;
	else if (opcode == OPCODE_IF_ELSE)
	{
		if (operand1)
		{
			return operand2;
		}
		else
		{
			return 0xFFFFFFFF;
		}
	}

	return 0;
}

void Parser()
{
    const uint8_t paramNumber = 0;

    for(uint8_t count = ParamTable[paramNumber].FormulaLength; count >= 1;)
    {
        uint8_t byte = ParamTable[paramNumber].Formula[--count];
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

void brute_force(int64_t NeedValue)
{
	uint8_t valid = 0;
	for (uint16_t d0 = 0; d0 < 256; d0++)
	{
		for (uint16_t d1 = 0; d1 < 256; d1++)
		{
			for (uint16_t d2 = 0; d2 < 256; d2++)
			{
				frame.Data[0] = d0; frame.Data[1] = d1; frame.Data[2] = d2;
				Do();
				if (Value == NeedValue)
				{
					valid = 1;
					break;
				}
			}
			if (valid == 1)
				break;
		}
		if (valid == 1)
			break;
	}
}

void method_dichotomy(int64_t NeedValue)
{
	uint32_t dataMinBorder = 0;
	uint32_t dataMaxBorder = 16777215;
	uint32_t dataGuess = 8388608;
	frame.Data[0] = dataGuess % 256;
	frame.Data[1] = (dataGuess >> 8) % 256;
	frame.Data[2] = (dataGuess >> 16) % 256;
	Do();
	uint32_t LastValue = Value;
	while (Value != NeedValue)
	{
		int64_t dif = Value - NeedValue;
		// Если нужное число слева
		if (dif > 0)
		{
			dataMaxBorder = dataGuess;
			dataGuess = (dataMaxBorder + dataMinBorder) >> 1;	// округляем в меньшую сторону
		}
		else
		{
			dataMinBorder = dataGuess;
			dataGuess = ((dataMaxBorder + dataMinBorder) >> 1) + 1;	// округляем в большую сторону
		}
		frame.Data[0] = dataGuess % 256;
		frame.Data[1] = (dataGuess >> 8) % 256;
		frame.Data[2] = (dataGuess >> 16) % 256;
		LastValue = Value;
		Do();
	}
}

void Test_calculation()
{
	std::cout << "Direct calculation:\n";
	frame.Data[0] = 9; frame.Data[1] = 3; frame.Data[2] = 0; Value = 0;
	std::cout << "Frame: ";
	for (uint8_t count = 0; count < 8; count++)
	{
		std::cout << frame.Data[count] + 0 << " ";
	}
	Do();
	std::cout << "\nValue: " << Value << "\n";



	std::cout << "\nInverse calculation:";
	int64_t NeedValue = 167772150;
	std::cout << "\nValue: " << NeedValue << "\n";
	std::cout << "Frame: ";
	brute_force(NeedValue);
	//method_dichotomy(NeedValue);
	for (uint8_t count = 0; count < 8; count++)
	{
		std::cout << frame.Data[count] + 0 << " ";
	}

}

void Show_formula()
{
    const uint8_t paramNumber = 0;
    std::cout << "Formula[0]:" << std::endl;
	std::cout << "Length = ";
    std::cout << ParamTable[paramNumber].FormulaLength + 0 << std::endl;
    for(uint8_t count = 1; count < ParamTable[paramNumber].FormulaLength; count++)
    {
        uint8_t byte = ParamTable[paramNumber].Formula[count];
        Show_byte(byte);
    }
}

void Show_byte(uint8_t byte)
{
    if(byte < 0x08)
        std::cout << "d" << byte + 0 << " ";
    else if(byte >= 0x80)
        std::cout << byte - 0x80 << " ";
    else if(byte == OPCODE_ADD)
        std::cout << "+ ";
    else if(byte == OPCODE_SUB)
        std::cout << "- ";
    else if(byte == OPCODE_DIV)
        std::cout << "/ ";
    else if(byte == OPCODE_MUL)
        std::cout << "* ";
    else if(byte == OPCODE_BIT_AND)
        std::cout << "& ";
    else if(byte == OPCODE_BIT_OR)
        std::cout << "| ";
    else if(byte == OPCODE_SHIFT_LEFT)
        std::cout << "<< ";
    else if(byte == OPCODE_SHIFT_RIGHT)
        std::cout << ">> ";
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
#include "obd_test.hpp"
#include <iostream>
#include <cstring>

/*
* @brief ������ ������� d0+5
*/
void TestOBD::CreateTreesForReverseElementary()
{
    ParamNumber = 0;
    ParamCount = 0;
    {// ADD
        ParamTable[ParamNumber].FormulaLength = 4;
        uint8_t arr[4] = { 0x01, OPCODE_LOG_OR, 0x00, 0x85 };
        memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
        ParamNumber++;
    }
    {// ADD
        ParamTable[ParamNumber].FormulaLength = 4;
        uint8_t arr[4] = { 0x01, 0x18, 0x00, 0x85 };
        memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
        ParamNumber++;
    }
    {// SUB
        ParamTable[ParamNumber].FormulaLength = 4;
        uint8_t arr[4] = { 0x01, 0x19, 0x00, 0x85 };
        memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
        ParamNumber++;
    }
    {// MUL
        ParamTable[ParamNumber].FormulaLength = 4;
        uint8_t arr[4] = { 0x01, 0x1A, 0x00, 0x85 };
        memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
        ParamNumber++;
    }
    {// DIV
        ParamTable[ParamNumber].FormulaLength = 4;
        uint8_t arr[4] = { 0x01, 0x1B, 0x00, 0x85 };
        memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
        ParamNumber++;
    }
    CreateTrees();
}


/*
* @brief ������ ������� ������� 
*/
void TestOBD::CreateTreeForReverseWithDifficulties()
{
    ParamNumber = 0;
    ParamCount = 0;
    {	// "(d0 | (d1 << 8) | (d2 << 16) ) * 10"
        ParamTable[ParamNumber].FormulaLength = 12;
        uint8_t arr[12] = { 0x01, 0x1A, 0x0D, 0x0D, 0x16, 0x02, 0x90, 0x16, 0x01, 0x88, 0x00, 0x8A };
        memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
        ParamNumber++;
    }
	{	// "( ( ( d2 &amp; 15 ) &lt;&lt; 16 ) | ( d1 &lt;&lt; 8 ) | d0 ) * 10"
		ParamTable[ParamNumber].FormulaLength = 14;
		uint8_t arr[14] = { 0x01, 0x1A, 0x0D, 0x0D, 0x16, 0x0F, 0x02, 0x8F, 0x90, 0x16, 0x01, 0x88, 0x00, 0x8A };
		memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
		ParamNumber++;
	}
	{	// "( 3 * d0 - 38 ) / 7"
		ParamTable[ParamNumber].FormulaLength = 8;
		uint8_t arr[8] = { 0x01, OPCODE_DIV, OPCODE_SUB, OPCODE_MUL, 0x83, 0x00, 0xA6, 0x87 };
		memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
		ParamNumber++;
	}
	{	// "( ( d2 &lt;&lt; 8 ) | d3 ) / ( 100 + 103 ) * 10"
		ParamTable[ParamNumber].FormulaLength = 12;
		uint8_t arr[12] = { 0x01, 0x1A, 0x1B, 0x0D, 0x16, 0x02, 0x88, 0x03, 0x18, 0xE4, 0xE7, 0x8A };
		memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
		ParamNumber++;
	}
	{	// "( d0 == 40 ) | ( d0 == 48 )"
		ParamTable[ParamNumber].FormulaLength = 8;
		uint8_t arr[8] = { 0x01, OPCODE_BIT_OR, OPCODE_EQU, 0x00, 0xA8, OPCODE_EQU, 0x00, 0xB0 };
		memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
		ParamNumber++;
	}
	// "( ( ( d4 &amp; 1 ) &lt;&lt; 8 ) | d5 ) * 100 / ( 4 * 100 + 7 )"

	// "( ( d4 &gt;&gt; 7 ) &amp; 1 ) &amp; ( ( ( d4 &gt;&gt; 6 ) &amp; 1 ) == 0)"

	{	// "( ( d1 &amp; 3 ) == 2 ) | ( ( d1 &amp; 3 ) == 3 )"
		ParamTable[ParamNumber].FormulaLength = 12;
		uint8_t arr[12] = { 0x01, 0x0D, 0x10, 0x82, 0x0F, 0x01, 0x83, 0x10, 0x83, 0x0F, 0x01, 0x83 };
		memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
		ParamNumber++;
	}
	{	// "( d2 == 41 ) * ( d3 / 2 ) "
		ParamTable[ParamNumber].FormulaLength = 8;
		uint8_t arr[8] = { 0x01, OPCODE_MUL, OPCODE_EQU, 0x02, 0xA9, OPCODE_DIV, 0x03, 0x82 };
		memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
		ParamNumber++;
	}
	{	// "( d0 == 2 ) * ( ( d5 &lt;&lt; 16 ) | ( d6 &lt;&lt; 8 ) | d7 ) * 10"
		ParamTable[ParamNumber].FormulaLength = 16;
		uint8_t arr[16] = { 0x01, 0x1A, 0x1A, 0x10, 0x00, 0x82, 0x0D, 0x0D, 0x16, 0x05, 0x90, 0x16, 0x06, 0x88, 0x07, 0x8A };
		memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
		ParamNumber++;
	}
	
    CreateTrees();
}


/*
* @brief ������ ������� ��� Audi A3 2013
*/
void TestOBD::CreateTreesForAudiA3_2013()
{
    ParamNumber = 0;
    ParamCount = 0;
    {	// "( d4 &amp; 1 ) == 0"
        ParamTable[ParamNumber].FormulaLength = 6;
        uint8_t arr[12] = { 0x01, 0x10, 0x0F, 0x04, 0x81, 0x80 };
        memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
        ParamNumber++;
    }
    {	// "( ( d3 &lt;&lt; 8 ) | d2 ) / 4"
        ParamTable[ParamNumber].FormulaLength = 8;
        uint8_t arr[8] = { 0x01, 0x1B, 0x0D, 0x16, 0x03, 0x88, 0x02, 0x84 };
        memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
        ParamNumber++;
    }
    {	// "( ( ( d2 &amp; 15 ) &lt;&lt; 16 ) | ( d1 &lt;&lt; 8 ) | d0 ) * 10"
        ParamTable[ParamNumber].FormulaLength = 12;
        uint8_t arr[12] = { 0x01, 0x1A, 0x0D, 0x0D, 0x16, 0x02, 0x90, 0x16, 0x01, 0x88, 0x00, 0x8A };
        memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
        ParamNumber++;
    }
    {	// "d1 - 60"
        ParamTable[ParamNumber].FormulaLength = 4;
        uint8_t arr[4] = { 0x01, OPCODE_SUB, 0x01, 0xBC};
        memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
        ParamNumber++;
    }
    {	// "( ( d1 &gt;&gt; 5 ) &amp; 1 ) == 1"
        ParamTable[ParamNumber].FormulaLength = 8;
        uint8_t arr[8] = { 0x01, OPCODE_EQU, OPCODE_BIT_AND, OPCODE_SHIFT_RIGHT, 0x01, 0x85, 0x81, 0x81};
        memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
        ParamNumber++;
    }
    {   // "( ( d1 &gt;&gt; 6 ) &amp; 1 ) == 1"
        ParamTable[ParamNumber].FormulaLength = 8;
        uint8_t arr[8] = { 0x01, OPCODE_EQU, OPCODE_BIT_AND, OPCODE_SHIFT_RIGHT, 0x01, 0x86, 0x81, 0x81};
        memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
        ParamNumber++;
    }
    {    // "d5 &amp; 127"
        ParamTable[ParamNumber].FormulaLength = 4;
        uint8_t arr[4] = { 0x01, OPCODE_BIT_AND, 0x05, 0xFF};
        memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
        ParamNumber++;
    }
    {    // "0"
        ParamTable[ParamNumber].FormulaLength = 2;
        uint8_t arr[2] = { 0x01, 0x80};
        memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
        ParamNumber++;
    }
    CreateTrees();
}


/*
* @brief ������������ 3-�� ������� ��������� ������� ������ ��� �������� �������
* @note ���������� ������ ������� � ��������
*/
void TestOBD::TestCalculationReverse()
{
    // �������������
    (*this).CreateTreeForReverseWithDifficulties();

    // ����� ������ � ��������
    int64_t needValue;

    for (ParamCount = 0; ParamCount < ParamNumber; ParamCount++)
    {
		for (uint8_t countOfValue = 0; countOfValue < 4; countOfValue++)
		{
			if (countOfValue == 0)
				needValue = 0;
			else if (countOfValue == 1)
				needValue = 1;
			else if (countOfValue == 2)
				needValue = 11;
			else if (countOfValue == 3)
				needValue = 40;
			ShowFormula();
			ShowTree();

			std::cout << "\n\nReverse calculation (brut force):";
			std::cout << "\nValue: " << needValue << "\n";
			std::cout << "Frame: ";
			memset(frame.Data, 0, 8);
			if (DoReverseCalculateWithBruteForce(needValue) == 0)
			{
				for (uint8_t count = 0; count < 8; count++)
					std::cout << frame.Data[count] + 0 << " ";
			}
			else
				std::cout << "There is no match!";

			std::cout << "\n\nReverse calculation (tree):";
			std::cout << "\nValue: " << needValue << "\n";
			std::cout << "Frame: ";
			memset(frame.Data, 0, 8);
			DoReverseCalculateWithTree(needValue);
			for (uint8_t count = 0; count < 8; count++)
				std::cout << frame.Data[count] + 0 << " ";

			std::cout << "\n\nReverse calculation (method dichotomy):";
			std::cout << "\nValue: " << needValue << "\n";
			std::cout << "Frame: ";
			memset(frame.Data, 0, 8);
			if (DoReverseCalculateWithMethodDichotomy(needValue) == 0)
			{
				for (uint8_t count = 0; count < 8; count++)
					std::cout << frame.Data[count] + 0 << " ";
			}
			else
				std::cout << "Out of time!";
			std::cout << "\n\n\n\n";
		}
    }
}

/*
* @brief ������� � �������� �������, ���������� �� �������������
*/
void TestOBD::ShowFormula()
{
	std::cout << "\nFormula[" << ParamCount + 0 << "]: " << std::endl;
	std::cout << "Length = " << ParamTable[ParamCount].FormulaLength + 0 << std::endl;
	for (uint8_t countOfByte = 1; countOfByte < ParamTable[ParamCount].FormulaLength; countOfByte++)
	{
		uint8_t byte = ParamTable[ParamCount].Formula[countOfByte];
		ShowByte(byte);
	}
}


/*
* @brief ������� � �������� ������������ ���� �������
*/
void TestOBD::ShowByte(uint8_t byte)
{
	if (byte < 0x08)						std::cout << "d" << byte + 0 << " ";
	else if (byte >= 0x80)					std::cout << byte - 0x80 << " ";
	else if (byte == OPCODE_LOG_OR)			std::cout << "|| ";
	else if (byte == OPCODE_LOG_AND)		std::cout << "&& ";
	else if (byte == OPCODE_LOG_NOT)		std::cout << "! ";
	else if (byte == OPCODE_BIT_NOT)		std::cout << "~ ";
	else if (byte == OPCODE_BIT_OR)			std::cout << "| ";
	else if (byte == OPCODE_BIT_XOR)		std::cout << "^ ";
	else if (byte == OPCODE_BIT_AND)		std::cout << "& ";
	else if (byte == OPCODE_EQU)			std::cout << "== ";
	else if (byte == OPCODE_NEQU)			std::cout << "<> ";
	else if (byte == OPCODE_LESS)			std::cout << "< ";
	else if (byte == OPCODE_MORE)			std::cout << "> ";
	else if (byte == OPCODE_LESS_EQU)		std::cout << "<= ";
	else if (byte == OPCODE_MORE_EQU)		std::cout << ">= ";
	else if (byte == OPCODE_SHIFT_LEFT)		std::cout << "<< ";
	else if (byte == OPCODE_SHIFT_RIGHT)	std::cout << ">> ";
	else if (byte == OPCODE_ADD)			std::cout << "+ ";
	else if (byte == OPCODE_SUB)			std::cout << "- ";
	else if (byte == OPCODE_MUL)			std::cout << "* ";
	else if (byte == OPCODE_DIV)			std::cout << "/ ";
	else if (byte == OPCODE_IF_ELSE)		std::cout << "IF-ELSE ";
	else
		std::cout << "? ";
}


/*
* @brief ������� � �������� ��������� ������, ������� � �������� ����, ���� ��������
*/
void TestOBD::ShowTree()
{
	Tree::Node* ptrNode = ParamTable[ParamCount].tree.GetBaseNode();
	std::cout << std::endl << "Tree:" << std::endl;
	if (ptrNode != nullptr)
	{
		ShowTreeNode(ptrNode);
	}
	else
		std::cout << std::endl << "Tree is empty";
}


/*
* @brief ���������� ������� � �������� ��������� ������
*/
void TestOBD::ShowTreeNode(Tree::Node* ptrNode)
{
	if (ptrNode != nullptr)
	{
		if (IsItOperator(ptrNode->Value))
		{
			ShowByte(ptrNode->Value);
			ShowTreeNode(ptrNode->ChildsArr[0]);
			ShowTreeNode(ptrNode->ChildsArr[1]);
		}
		else
			std::cout << "   ";
	}
}

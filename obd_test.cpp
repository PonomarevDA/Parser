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
* @brief ������ ������� (d0 | (d1 << 8) | (d2 << 16) ) * 10
*/
void TestOBD::CreateTreeForReverseWithDifficulties()
{
	ParamNumber = 0;
	ParamCount = 0;
	{
		ParamTable[ParamNumber].FormulaLength = 12;
		uint8_t arr[12] = { 0x01, 0x1A, 0x0D, 0x0D, 0x16, 0x02, 0x90, 0x16, 0x01, 0x88, 0x00, 0x8A };
		memcpy(ParamTable[ParamNumber].Formula, arr, ParamTable[ParamNumber].FormulaLength);
		ParamNumber++;
	}
    
	CreateTrees();
}


/*
* @brief ������ ������� ��� Audi A3 2013
*/
void TestOBD::CreateTreeForAudiA3_2013()
{
	ParamNumber = 0;
	ParamCount = 0;
	{
		ParamTable[ParamNumber].FormulaLength = 6;
		uint8_t arr[12] = { 0x01, 0x10, 0x0F, 0x04, 0x81, 0x00 };
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
    (*this).CreateTreeForAudiA3_2013();

	// ����� ������ � ��������
    int64_t NeedValue = 1;

	for (ParamCount = 0; ParamCount < ParamNumber; ParamCount++)
	{
		ShowFormula();
		ShowTree();

		std::cout << "\n\nReverse calculation (brut force):";
		std::cout << "\nValue: " << NeedValue << "\n";
		std::cout << "Frame: ";
		memset(frame.Data, 0, 8);
		DoReverseCalculateWithBruteForce(NeedValue);
		for (uint8_t count = 0; count < 8; count++)
			std::cout << frame.Data[count] + 0 << " ";

		std::cout << "\n\nReverse calculation (tree):";
		std::cout << "\nValue: " << NeedValue << "\n";
		std::cout << "Frame: ";
		memset(frame.Data, 0, 8);
		DoReverseCalculateWithTree(NeedValue);
		for (uint8_t count = 0; count < 8; count++)
			std::cout << frame.Data[count] + 0 << " ";

		std::cout << "\n\nReverse calculation (method dichotomy) [work only with d0,d1,d2,d3]:";
		std::cout << "\nValue: " << NeedValue << "\n";
		std::cout << "Frame: ";
		memset(frame.Data, 0, 8);
		DoReverseCalculateWithMethodDichotomy(NeedValue);
		for (uint8_t count = 0; count < 8; count++)
			std::cout << frame.Data[count] + 0 << " ";
		std::cout << "\n\n\n\n";
	}
}

/*
* @brief �������� ����. ��������� ������ � �������� ������� ������
* @return ���������� ������� � ��������
*/
void TestOBD::TestCalculations()
{
    std::cout << "Direct calculation:\n";
    frame.Data[0] = 9; frame.Data[1] = 3; frame.Data[2] = 0; Value = 0;
    std::cout << "Frame: ";
    for (uint8_t count = 0; count < 8; count++)
    {
        std::cout << frame.Data[count] + 0 << " ";
    }
    DoDirectCalculate();
    std::cout << "\nValue: " << Value << "\n";



    std::cout << "\nReverse calculation:";
    int64_t NeedValue = 167772140;
    std::cout << "\nValue: " << NeedValue << "\n";
    std::cout << "Frame: ";
    DoReverseCalculateWithBruteForce(NeedValue);
    //Do_reverse_calculate_with_method_dichotomy(NeedValue);
    for (uint8_t count = 0; count < 8; count++)
        std::cout << frame.Data[count] + 0 << " ";
}

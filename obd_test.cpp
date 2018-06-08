#include "obd_test.hpp"
#include "obd.hpp"
#include <iostream>

extern ParamTable_t ParamTable[1];
extern uint8_t ParamCount;
extern Frame frame;
extern uint32_t Value;

/*
* @brief Задать формулу d0+5
*/
void TestOBD::Create_tree_for_reverse_ADD()
{
	const uint8_t paramNumber = 0;
	ParamTable[0].FormulaLength = 4;
	uint8_t arr[4] = { 0x01, 0x18, 0x00, 0x85 };
	memcpy(ParamTable[paramNumber].Formula, arr, ParamTable[paramNumber].FormulaLength);
	Create_tree();
}


/*
* @brief Задать формулу d0-5
*/
void TestOBD::Create_tree_for_reverse_SUB()
{
	const uint8_t paramNumber = 0;
	ParamTable[0].FormulaLength = 4;
	uint8_t arr[4] = { 0x01, 0x19, 0x00, 0x85 };
	memcpy(ParamTable[paramNumber].Formula, arr, ParamTable[paramNumber].FormulaLength);
	Create_tree();
}


/*
* @brief Задать формулу d0*5
*/
void TestOBD::Create_tree_for_reverse_MUL()
{
	const uint8_t paramNumber = 0;
	ParamTable[0].FormulaLength = 4;
	uint8_t arr[4] = { 0x01, 0x1A, 0x00, 0x85 };
	memcpy(ParamTable[paramNumber].Formula, arr, ParamTable[paramNumber].FormulaLength);
	Create_tree();
}


/*
* @brief Задать формулу d0/5
*/
void TestOBD::Create_tree_for_reverse_DIV()
{
	const uint8_t paramNumber = 0;
	ParamTable[0].FormulaLength = 4;
	uint8_t arr[4] = { 0x01, 0x1B, 0x00, 0x85 };
	memcpy(ParamTable[paramNumber].Formula, arr, ParamTable[paramNumber].FormulaLength);
	Create_tree();
}


/*
* @brief Тестирование 3-ех методов обратного решения задачи для заданной формулы
* @note Результаты тестов выводит в терминал
*/
void TestOBD::Test_calculation_reverse()
{
	(*this).Create_tree_for_reverse_DIV();
	Show_formula();
	Show_tree();

	std::cout << "\n\nReverse calculation (brut force):";
	int64_t NeedValue = 100;
	std::cout << "\nValue: " << NeedValue << "\n";
	std::cout << "Frame: ";
	Do_reverse_calculate_with_brute_force(NeedValue);
	for (uint8_t count = 0; count < 8; count++)
		std::cout << frame.Data[count] + 0 << " ";

	std::cout << "\n\nReverse calculation (tree):";
	NeedValue = 100;
	std::cout << "\nValue: " << NeedValue << "\n";
	std::cout << "Frame: ";
	Do_reverse_calculate_with_tree(NeedValue);
	for (uint8_t count = 0; count < 8; count++)
		std::cout << frame.Data[count] + 0 << " ";

	std::cout << "\n\nReverse calculation (method dichotomy):";
	NeedValue = 100;
	std::cout << "\nValue: " << NeedValue << "\n";
	std::cout << "Frame: ";
	Do_reverse_calculate_with_method_dichotomy(NeedValue);
	for (uint8_t count = 0; count < 8; count++)
		std::cout << frame.Data[count] + 0 << " ";
}

/*
* @brief Тестовый кейс. Выполняет прямое и обратные решения задачи
* @return Результаты выводит в терминал
*/
void TestOBD::Test_calculations()
{
	std::cout << "Direct calculation:\n";
	frame.Data[0] = 9; frame.Data[1] = 3; frame.Data[2] = 0; Value = 0;
	std::cout << "Frame: ";
	for (uint8_t count = 0; count < 8; count++)
	{
		std::cout << frame.Data[count] + 0 << " ";
	}
	Do_direct_calculate();
	std::cout << "\nValue: " << Value << "\n";



	std::cout << "\nReverse calculation:";
	int64_t NeedValue = 167772140;
	std::cout << "\nValue: " << NeedValue << "\n";
	std::cout << "Frame: ";
	Do_reverse_calculate_with_brute_force(NeedValue);
	//Do_reverse_calculate_with_method_dichotomy(NeedValue);
	for (uint8_t count = 0; count < 8; count++)
	{
		std::cout << frame.Data[count] + 0 << " ";
	}

}
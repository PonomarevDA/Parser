#include "obd_test.hpp"
#include <iostream>
#include <cstring>

/*
* @brief Задать формулу d0+5
*/
void TestOBD::CreateTreeForReverseADD()
{
    const uint8_t paramNumber = 0;
    ParamTable[0].FormulaLength = 4;
    uint8_t arr[4] = { 0x01, 0x18, 0x00, 0x85 };
    memcpy(ParamTable[paramNumber].Formula, arr, ParamTable[paramNumber].FormulaLength);
    CreateTree();
}


/*
* @brief Задать формулу d0-5
*/
void TestOBD::CreateTreeForReverseSUB()
{
    const uint8_t paramNumber = 0;
    ParamTable[0].FormulaLength = 4;
    uint8_t arr[4] = { 0x01, 0x19, 0x00, 0x85 };
    memcpy(ParamTable[paramNumber].Formula, arr, ParamTable[paramNumber].FormulaLength);
    CreateTree();
}


/*
* @brief Задать формулу d0*5
*/
void TestOBD::CreateTreeForReverseMUL()
{
    const uint8_t paramNumber = 0;
    ParamTable[0].FormulaLength = 4;
    uint8_t arr[4] = { 0x01, 0x1A, 0x00, 0x85 };
    memcpy(ParamTable[paramNumber].Formula, arr, ParamTable[paramNumber].FormulaLength);
    CreateTree();
}


/*
* @brief Задать формулу d0/5
*/
void TestOBD::CreateTreeForReverseDIV()
{
    const uint8_t paramNumber = 0;
    ParamTable[0].FormulaLength = 4;
    uint8_t arr[4] = { 0x01, 0x1B, 0x00, 0x85 };
    memcpy(ParamTable[paramNumber].Formula, arr, ParamTable[paramNumber].FormulaLength);
    CreateTree();
}


/*
* @brief Задать формулу d0/5
*/
void TestOBD::CreateTreeForReverseWithDifficulties()
{
    const uint8_t paramNumber = 0;
    ParamTable[0].FormulaLength = 12;
    uint8_t arr[12] = { 0x01, 0x1A, 0x0D, 0x0D, 0x16, 0x02, 0x90, 0x16, 0x01, 0x88, 0x00, 0x8A };
    memcpy(ParamTable[paramNumber].Formula, arr, ParamTable[paramNumber].FormulaLength);
    CreateTree();
}


/*
* @brief Тестирование 3-ех методов обратного решения задачи для заданной формулы
* @note Результаты тестов выводит в терминал
*/
void TestOBD::TestCalculationReverse()
{
    (*this).CreateTreeForReverseWithDifficulties();
    ShowFormula();
    ShowTree();
    int64_t NeedValue = 5000;

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
}

/*
* @brief Тестовый кейс. Выполняет прямое и обратные решения задачи
* @return Результаты выводит в терминал
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

#include "obd_test.hpp"
#include <iostream>
#include <cstring>

/*
* @brief Задать формулу d0+5
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
* @brief Задать формулу (d0 | (d1 << 8) | (d2 << 16) ) * 10
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
* @brief Задать формулы для Audi A3 2013
*/
void TestOBD::CreateTreesForAudiA3_2013()
{
    ParamNumber = 0;
    ParamCount = 0;
    {	// "( d4 &amp; 1 ) == 0"
        ParamTable[ParamNumber].FormulaLength = 6;
        uint8_t arr[12] = { 0x01, 0x10, 0x0F, 0x04, 0x81, 0x00 };
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
* @brief Тестирование 3-ех методов обратного решения задачи для заданной формулы
* @note Результаты тестов выводит в терминал
*/
void TestOBD::TestCalculationReverse()
{
    // Инициализация
    (*this).CreateTreesForAudiA3_2013();

    // Вывод данных в терминал
    int64_t needValue = 50;

    for (ParamCount = 0; ParamCount < ParamNumber; ParamCount++)
    {
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
        if ( DoReverseCalculateWithMethodDichotomy(needValue) == 0)
        {
            for (uint8_t count = 0; count < 8; count++)
                std::cout << frame.Data[count] + 0 << " ";
        }
        else
            std::cout << "Out of time!";
        std::cout << "\n\n\n\n";
    }
}

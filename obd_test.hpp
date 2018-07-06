#ifndef __OBD_TEST
#define __OBD_TEST

#include "obd.hpp"

/**
* @brief Тестирование методов обратного расчета
*/
class TestOBD: public OBD
{
private:
    /// Метод для тестирования
    void TestCalculationReverse(int64_t needValue, uint8_t paramCount);

    /// Методы иллюстрации результатов алгоритмов в терминале:
    void ShowFormula(uint8_t paramCount);
    void ShowByte(uint8_t byte);
    void ShowTree(uint8_t paramCount);
    void ShowTreeNode(Tree::Node*);
public:
    /// Тестовые кейсы
    void TestReverseCalculateElementary();
    void TestReverseCalculateForAudiA3_2013();
    void TestReverseCalculateWithDifficulties();
};


#endif // __OBD

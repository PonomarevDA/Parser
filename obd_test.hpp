#ifndef __OBD_TEST
#define __OBD_TEST

#include "obd.hpp"

class TestOBD: public OBD
{
private:
    /// Метод для тестирования
    void TestCalculationReverse(int64_t needValue);

    /// Методы иллюстрации результатов алгоритмов в терминале:
    void ShowFormula();
    void ShowByte(uint8_t byte);
    void ShowTree();
    void ShowTreeNode(Tree::Node*);
public:
    /// Тестовые кейсы
    void TestReverseCalculateElementary();
    void TestReverseCalculateForAudiA3_2013();
    void TestReverseCalculateWithDifficulties();
};


#endif // __OBD

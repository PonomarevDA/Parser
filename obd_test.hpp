#ifndef __OBD_TEST
#define __OBD_TEST

#include "obd.hpp"

/**
* @brief ������������ ������� ��������� �������
*/
class TestOBD: public OBD
{
private:
    /// ����� ��� ������������
    void TestCalculationReverse(int64_t needValue, uint8_t paramCount);

    /// ������ ����������� ����������� ���������� � ���������:
    void ShowFormula(uint8_t paramCount);
    void ShowByte(uint8_t byte);
    void ShowTree(uint8_t paramCount);
    void ShowTreeNode(Tree::Node*);
public:
    /// �������� �����
    void TestReverseCalculateElementary();
    void TestReverseCalculateForAudiA3_2013();
    void TestReverseCalculateWithDifficulties();
};


#endif // __OBD

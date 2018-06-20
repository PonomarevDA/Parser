#ifndef __OBD_TEST
#define __OBD_TEST

#include "obd.hpp"

class TestOBD: public OBD
{
private:
    /// ����� ��� ������������
    void TestCalculationReverse(int64_t needValue);

    /// ������ ����������� ����������� ���������� � ���������:
    void ShowFormula();
    void ShowByte(uint8_t byte);
    void ShowTree();
    void ShowTreeNode(Tree::Node*);
public:
    /// �������� �����
    void TestReverseCalculateElementary();
    void TestReverseCalculateForAudiA3_2013();
    void TestReverseCalculateWithDifficulties();
};


#endif // __OBD

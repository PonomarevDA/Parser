#ifndef __OBD_TEST
#define __OBD_TEST

#include "obd.hpp"

class TestOBD: public OBD
{
private:
	/// �������� �����
    void CreateTreesForReverseElementary();
    void CreateTreesForAudiA3_2013();
    void CreateTreeForReverseWithDifficulties();
public:
	/// ������ ������������
    void TestCalculationReverse();
    void TestCalculations();

	/// ������ ����������� ����������� ���������� � ���������:
	void ShowFormula();
	void ShowByte(uint8_t byte);
	void ShowTree();
	void ShowTreeNode(Tree::Node*);
};


#endif // __OBD

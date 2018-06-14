#ifndef __OBD_TEST
#define __OBD_TEST

#include "obd.hpp"

class TestOBD: public OBD
{
private:
	/// Тестовые кейсы
    void CreateTreesForReverseElementary();
    void CreateTreesForAudiA3_2013();
    void CreateTreeForReverseWithDifficulties();

	/// Методы иллюстрации результатов алгоритмов в терминале:
	void ShowFormula();
	void ShowByte(uint8_t byte);
	void ShowTree();
	void ShowTreeNode(Tree::Node*);
public:
	/// Методы тестирования
    void TestCalculationReverse();
    void TestCalculations();
};


#endif // __OBD

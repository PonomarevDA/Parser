#ifndef __OBD_TEST
#define __OBD_TEST

#include "obd.hpp"

class TestOBD: public OBD
{
private:
    void CreateTreeForReverseADD();
    void CreateTreeForReverseSUB();
    void CreateTreeForReverseMUL();
    void CreateTreeForReverseDIV();
    void CreateTreeForReverseWithDifficulties();
public:
    void TestCalculationReverse();
    void TestCalculations();
};


#endif // __OBD

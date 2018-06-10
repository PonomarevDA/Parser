/*
* @File ���� � ����������� ������� ������ Stack
*/

#include "data_structures.hpp"


/*
* @File �������� ����� - push
* @param ptrNode - ��������� �� ����
*/
void Stack::Push(Tree::Node* ptrNode)
{
    if(Size < MaxSize)
        Arr[Size++] = ptrNode;
}


/*
* @File �������� ����� - pop
* @return ��������� �� ����
*/
Tree::Node* Stack::Pop()
{
    if(Size != 0)
        return Arr[--Size];
    return nullptr;
}

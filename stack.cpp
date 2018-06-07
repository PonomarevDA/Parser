/*
* @File ���� � ����������� ������� ������ Stack
*/
#include "data_structures.hpp"
#include "obd.hpp"


void Stack::push(Tree::Node* ptrNode)
{
    arr[size++] = ptrNode;
}

Tree::Node* Stack::pop()
{
    return arr[--size];
}

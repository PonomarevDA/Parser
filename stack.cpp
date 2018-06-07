/*
* @File Файл с реализацией методов класса Stack
*/
#include "data_structures.hpp"
#include "obd.hpp"


void Stack::Push(Tree::Node* ptrNode)
{
	if(Size < MaxSize)
		Arr[Size++] = ptrNode;
}

Tree::Node* Stack::Pop()
{
	if(Size != 0)
		return Arr[--Size];
	return nullptr;
}

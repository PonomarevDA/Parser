/*
* @File Файл с реализацией методов класса Stack
*/
#include "data_structures.hpp"
#include "obd.hpp"


void Stack::push(Tree::Node* ptrNode)
{
	if(Size < MaxSize)
		arr[Size++] = ptrNode;
}

Tree::Node* Stack::pop()
{
	if(Size != 0)
		return arr[--Size];
	return nullptr;
}

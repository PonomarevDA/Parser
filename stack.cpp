/**
* @File Файл с реализацией методов класса Stack
*/

#include "data_structures.hpp"


/**
* @File Операция стека - push
* @param ptrNode - указатель на узел
*/
void Stack::Push(Tree::Node* const ptrNode)
{
    if(Size < MaxSize)
        Arr[Size++] = ptrNode;
}


/**
* @File Операция стека - pop
* @return указатель на узел, если стек не пуст, иначе nullptr
*/
Tree::Node* Stack::Pop()
{
    if(Size != 0)
        return Arr[--Size];
    return nullptr;
}

/**
* @File Файл с реализацией методов класса Tree
* @note Данное дерево предназначено для проведения разбора формул и выполнения
* обратного вычисления.
*/

#include "data_structures.hpp"
#include <cstring>


/**
* @brief Добавить узел нижнего уровня, т.е. у которого никогда не будет потомков
* @param value - значение узла
* @return указатель на добавленный узел, если памяти хватает, иначе nullptr
*/
Tree::Node* Tree::AddNodeLower(const uint8_t value)
{
    if (Size < MaxSize)
    {
        ArrOfNodes[Size].Value = value;
        return &ArrOfNodes[Size++];
    }
    return nullptr;
}


/**
* @brief Добавить узел "родитель", т.е. у которого есть дети
* @param value - значение узла
* @param childsArr - указатель указателей на потомков
* @param childsNum - количество потомков
* @return указатель на добавленный узел, если памяти хватает, иначе nullptr
*/
Tree::Node* Tree::AddNodeParent(uint8_t value, Node** childsArr, int8_t childsNum)
{
    if (Size < MaxSize)
    {
        ArrOfNodes[Size].Value = value;
        ArrOfNodes[Size].ChildsCount = childsNum;
        while(--childsNum >= 0)
        {
            childsArr[childsNum]->Parent = &ArrOfNodes[Size];
            ArrOfNodes[Size].ChildsArr[childsNum] = childsArr[childsNum];
        }
        return &ArrOfNodes[Size++];
    }
    return nullptr;
}


/**
* @brief Получить указатель на самый верхний узел (родитель всех узлов)
* @return указатель на самый верхний узел, если он существует, иначе nullptr
*/
Tree::Node* Tree::GetBaseNode()
{
	Tree::Node* node = &ArrOfNodes[0];
	if (node == nullptr)
		return node;
	while (node->Parent != nullptr)
		node = node->Parent;
	return node;
}


/**
* @brief Получить текущий размер дерева (текущее кол-во узлов)
* @return текущий размер дерева
*/
uint8_t Tree::GetSize() const
{
    return Size;
}


/**
* @brief Получить максимальный размер дерева (максимальное кол-во узлов)
* @return максимальный размер дерева
*/
uint8_t Tree::GetMaxSize() const
{
    return MaxSize;
}


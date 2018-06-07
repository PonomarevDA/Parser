/*
* @File Файл с реализацией методов класса Tree
*/
#include "data_structures.hpp"
#include "obd.hpp"
#include <cstring>

/*
* @brief Добавить узел нижнего уровня, т.е. у которого никогда не будет детей
* @param value - значение узла
* @note Если памяти не хватает - игнорируем
*/
Tree::Node* Tree::Add_node_lower(uint8_t value)
{
    if (Size < MaxSize)
    {
        ArrOfNodes[Size].Value = value;
        return &ArrOfNodes[Size++];
    }
    return nullptr;
}


/*
* @brief Добавить узел "родитель", т.е. у которого есть дети
* @param value - значение узла
* @param value - значение узла
* @return указатель на узел, иначе nullptr
*/
Tree::Node* Tree::Add_node_parent(uint8_t value, Node** childsArr, uint8_t childsCount)
{
    if (Size < MaxSize)
    {
        ArrOfNodes[Size].Value = value;
        ArrOfNodes[Size].ChildsArr = childsArr;
        ArrOfNodes[Size].ChildsCount = childsCount;
        while(childsCount-- != 0)
            (*(childsArr)++)->Parent = &ArrOfNodes[Size];

        return &ArrOfNodes[Size++];
    }
    return nullptr;
}


/*
* @brief Получить текущий размер дерева (текущее кол-во узлов)
* @return текущий размер дерева
*/
uint8_t Tree::Get_size()
{
    return Size;
}


/*
* @brief Получить максимальный размер дерева (максимальное кол-во узлов)
* @return максимальный размер дерева
*/
uint8_t Tree::Get_max_size()
{
    return MaxSize;
}


/*
* @brief Получить указатель на самый верхний узел (родитель всех узлов)
* @return указатель на самый верхний узел
*/
Tree::Node* Tree::Get_base_node()
{
    Tree::Node* node = &ArrOfNodes[0];
	if (node == nullptr)
		return node;
    while(node->Parent != nullptr)
    {
        node = node->Parent;
    }
    return node;
}
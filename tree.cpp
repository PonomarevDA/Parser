/*
* @File Файл с реализацией методов класса Tree
*/
#include "stdafx.h"
#include "data_structures.hpp"
#include "obd.hpp"
#include <cstring>

/*
* @brief Добавить узел нижнего уровня, т.е. у которого никогда не будет детей
* @param value - значение узла
* @note Если памяти не хватает - игнорируем
*/
void Tree::add_node_lower(uint8_t value)
{
    if (Size < MaxSize)
        ArrOfNodes[Size++].Value = value;
}


/*
* @brief Добавить узел "родитель", т.е. у которого есть дети
* @param value - значение узла
* @param value - значение узла
* @note Если памяти не хватает - игнорируем
*/
void Tree::add_node_parent(uint8_t value, Node** childsArr, uint8_t childsCount)
{
    if (Size < MaxSize)
    {
        ArrOfNodes[Size].Value = value;
        ArrOfNodes[Size].ChildsArr = childsArr;
        ArrOfNodes[Size++].ChildsCount = childsCount;
    }
}


/*
* @brief Получить значение узла
*/
/*
uint8_t Tree::get_value_of_node(Node* ptrNode)
{
    return ArrOfNodes[numberOfNode].get_value();
}
int8_t Tree::get_parent_of_node(uint64_t node)
{
    return ArrOfNodes[node].get_parent();
}
*/

/*
* @brief Получить размер дерева (кол-во узлов)
*/
uint8_t Tree::get_size()
{
    return Size;
}


/*
* @brief Получить размер дерева (кол-во узлов)
*/
uint8_t Tree::get_max_size()
{
    return MaxSize;
}


/*
* @File ‘айл с реализацией методов класса Tree
*/
#include "data_structures.hpp"
#include "obd.hpp"
#include <cstring>

/*
* @brief ƒобавить узел нижнего уровн€, т.е. у которого никогда не будет детей
* @param value - значение узла
* @note ≈сли пам€ти не хватает - игнорируем
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
* @brief ƒобавить узел "родитель", т.е. у которого есть дети
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
* @brief ѕолучить размер дерева (кол-во узлов)
*/
uint8_t Tree::Get_size()
{
    return Size;
}


/*
* @brief ѕолучить размер дерева (кол-во узлов)
*/
uint8_t Tree::Get_max_size()
{
    return MaxSize;
}


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

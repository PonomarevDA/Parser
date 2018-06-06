/*
* @File ���� � ����������� ������� ������ Tree
*/
#include "stdafx.h"
#include "data_structures.hpp"
#include "obd.hpp"
#include <cstring>

/*
* @brief �������� ���� ������� ������, �.�. � �������� ������� �� ����� �����
* @param value - �������� ����
* @note ���� ������ �� ������� - ����������
*/
Tree::Node* Tree::add_node_lower(uint8_t value)
{
    if (Size < MaxSize)
    {
        ArrOfNodes[Size].Value = value;
        return &ArrOfNodes[Size++];
    }
    return nullptr;
}


/*
* @brief �������� ���� "��������", �.�. � �������� ���� ����
* @param value - �������� ����
* @param value - �������� ����
* @return ��������� �� ����, ����� nullptr
*/
Tree::Node* Tree::add_node_parent(uint8_t value, Node** childsArr, uint8_t childsCount)
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
* @brief �������� ������ ������ (���-�� �����)
*/
uint8_t Tree::get_size()
{
    return Size;
}


/*
* @brief �������� ������ ������ (���-�� �����)
*/
uint8_t Tree::get_max_size()
{
    return MaxSize;
}


Tree::Node* Tree::get_base_node()
{
    Tree::Node* node = &ArrOfNodes[0];
    while(node->Parent != nullptr)
    {
        node = node->Parent;
    }
    return node;
}

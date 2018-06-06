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
void Tree::add_node_lower(uint8_t value)
{
    if (Size < MaxSize)
        ArrOfNodes[Size++].Value = value;
}


/*
* @brief �������� ���� "��������", �.�. � �������� ���� ����
* @param value - �������� ����
* @param value - �������� ����
* @note ���� ������ �� ������� - ����������
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
* @brief �������� �������� ����
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


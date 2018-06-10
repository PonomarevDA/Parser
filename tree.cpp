/*
* @File ���� � ����������� ������� ������ Tree
* @note ������ ������ ������������� ��� ���������� ������� ������ � ����������
* ��������� ����������.
*/

#include "data_structures.hpp"
#include <cstring>


/*
* @brief �������� ���� ������� ������, �.�. � �������� ������� �� ����� �����
* @param value - �������� ����
* @note ���� ������ �� ������� - ����������
*/
Tree::Node* Tree::AddNodeLower(uint8_t value)
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
Tree::Node* Tree::AddNodeParent(uint8_t value, Node** childsArr, uint8_t childsCount)
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
* @brief �������� ������� ������ ������ (������� ���-�� �����)
* @return ������� ������ ������
*/
uint8_t Tree::GetSize()
{
    return Size;
}


/*
* @brief �������� ������������ ������ ������ (������������ ���-�� �����)
* @return ������������ ������ ������
*/
uint8_t Tree::GetMaxSize()
{
    return MaxSize;
}


/*
* @brief �������� ��������� �� ����� ������� ���� (�������� ���� �����)
* @return ��������� �� ����� ������� ����
*/
Tree::Node* Tree::GetBaseNode()
{
    Tree::Node* node = &ArrOfNodes[0];
    if (node == nullptr)
        return node;
    while(node->Parent != nullptr)
        node = node->Parent;
    return node;
}

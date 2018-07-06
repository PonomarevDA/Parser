/**
* @File ���� � ����������� ������� ������ Tree
* @note ������ ������ ������������� ��� ���������� ������� ������ � ����������
* ��������� ����������.
*/

#include "data_structures.hpp"
#include <cstring>


/**
* @brief �������� ���� ������� ������, �.�. � �������� ������� �� ����� ��������
* @param value - �������� ����
* @return ��������� �� ����������� ����, ���� ������ �������, ����� nullptr
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
* @brief �������� ���� "��������", �.�. � �������� ���� ����
* @param value - �������� ����
* @param childsArr - ��������� ���������� �� ��������
* @param childsNum - ���������� ��������
* @return ��������� �� ����������� ����, ���� ������ �������, ����� nullptr
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
* @brief �������� ��������� �� ����� ������� ���� (�������� ���� �����)
* @return ��������� �� ����� ������� ����, ���� �� ����������, ����� nullptr
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
* @brief �������� ������� ������ ������ (������� ���-�� �����)
* @return ������� ������ ������
*/
uint8_t Tree::GetSize() const
{
    return Size;
}


/**
* @brief �������� ������������ ������ ������ (������������ ���-�� �����)
* @return ������������ ������ ������
*/
uint8_t Tree::GetMaxSize() const
{
    return MaxSize;
}


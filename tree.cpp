/*
* @File ���� � ����������� ������� ������ Tree
*/
#include "stdafx.h"
#include "data_structures.hpp"
#include "obd.hpp"
#include <cstring>

/************************************ class Node ************************************/
/*
* @brief ���������� ��������� �� ������ ������� ����
*/
int64_t Tree::Node::get_parent()
{
	return Parent;
}

/*
* @brief ���������� �������� ������� ����
*/
int64_t Tree::Node::get_value()
{
	return Value;
}
/************************************ class Node ************************************/


/************************************ class Tree ************************************/

/*
* @brief �������� ����
* @note ���� ������ ����� �����, ��������� ��� � 1.5 ����
*/
void Tree::add_node(int64_t value, int64_t parentNumber)
{
	// ���� ������ ������������ - ���������������� ����������� ������ �������
	if (Size >= MaxSize)
	{
		uint64_t lastMaxSize = MaxSize;
		MaxSize = MaxSize * 3 >> 1;
		Node* buffer = new Node[lastMaxSize];
		memcpy(buffer, ArrOfNodes, lastMaxSize);
		delete[] ArrOfNodes;
		ArrOfNodes = new Node[MaxSize];
		memcpy(ArrOfNodes, buffer, MaxSize);
	}
	ArrOfNodes[Size++] = Node(value, parentNumber);
}

/*
* @brief �������� �������� ����
*/
int64_t Tree::get_value_of_node(uint64_t numberOfNode)
{
	return ArrOfNodes[numberOfNode].get_value();
}
int64_t Tree::get_parent_of_node(uint64_t node)
{
	return ArrOfNodes[node].get_parent();
}

uint64_t get_number_of_root();


/*
* @brief �������� ������ ������ (���-�� �����)
*/
uint64_t Tree::get_size()
{
	return Size;
}


/*
* @brief �������� ������ ������ (���-�� �����)
*/
uint64_t Tree::get_max_size()
{
	return MaxSize;
}

/************************************ class Tree ************************************/
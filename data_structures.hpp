#ifndef DATASTRUCTURES
#define DATASTRUCTURES

#include "obd.hpp"

/*
* @brief —тек заранее заданного размера
*/
class Stack
{
public:
	Stack() : size(0) {}
	void push(Tree::Node*);
	Tree::Node* pop();
private:
	Tree::Node* arr[32];
	uint8_t size;
};

/*
* @brief ƒерево динамического размера, узлы которого пронумерованы
*/
class Tree
{
public:
	/*
	* @brief ”зел дерева: хранит байт данных, указатель на родител€, указатель указателей на потомков и их кол-во
	*/
	class Node
	{
	public:
		enum
		{
			NODE_ROOT = NULL,
		};
		Node() : Byte(0), Parent(NULL), ChildArr(NULL), ChildCount(0) {}
		Node(uint8_t byte, Node* parent) : Byte(byte), Parent(parent) {}
		uint8_t get_value();
		Node* get_parent();
	private:
		uint8_t Byte;
		Node* Parent;
		Node** ChildArr;
		uint8_t ChildCount;
	};

	Tree() : Root(0), Size(0), MaxSize(10) { ArrOfNodes = new Node[10]; }
	void add_node(int64_t, int64_t);
	int64_t get_value_of_node(uint64_t);
	int64_t get_parent_of_node(uint64_t);

	uint64_t get_number_of_root();
	uint64_t get_size();
	uint64_t get_max_size();
private:
	Node * ArrOfNodes;
	uint64_t Root;
	uint64_t Size;
	uint64_t MaxSize;
};

#endif // DATASTRUCTURES

#ifndef DATASTRUCTURES
#define DATASTRUCTURES

#include "obd.hpp"

/*
* @brief ƒерево динамического размера, узлы которого пронумерованы
*/
class Tree
{
public:
    /*
    * @brief ”зел дерева: хранит байт данных, кол-во потомков, указатель указателей на них и указатель на родител€
    */
    struct Node
    {
        Node() : Value(0), ChildsCount(0), ChildsArr(nullptr), Parent(nullptr) {}
        uint8_t Value;
        uint8_t ChildsCount;
        Node** ChildsArr;
        Node* Parent;
    };

    Tree() : Size(0), MaxSize(10) { ArrOfNodes = new Node[10]; }
    Tree(uint8_t maxSize) : Size(0), MaxSize(maxSize) { ArrOfNodes = new Node[MaxSize]; }

    void add_node_lower(uint8_t);
    void add_node_parent(uint8_t, Node**, uint8_t);

    uint8_t get_size();
    uint8_t get_max_size();
private:
    Node* ArrOfNodes;
    uint8_t Size;
    uint8_t MaxSize;
};


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

#endif // DATASTRUCTURES

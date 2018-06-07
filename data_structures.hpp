#ifndef DATASTRUCTURES
#define DATASTRUCTURES

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned int uint32_t;

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

    //Tree() : Size(0), MaxSize(10) { ArrOfNodes = new Node[33]; }
    //Tree(uint8_t maxSize) : Size(0), MaxSize(maxSize) { ArrOfNodes = new Node[MaxSize]; }
    Tree() : Size(0), MaxSize(10) {}
    Tree(uint8_t maxSize) : Size(0), MaxSize(maxSize) {}

    Node* add_node_lower(uint8_t);
    Node* add_node_parent(uint8_t, Node**, uint8_t);
    Node* get_base_node();

    uint8_t get_size();
    uint8_t get_max_size();
private:
    Node ArrOfNodes[32];
    uint8_t Size;
    uint8_t MaxSize;
};


/*
* @brief —тек заранее заданного размера - 32 байта
*/
class Stack
{
public:
    Stack() : Size(0), MaxSize(32) {}
    void push(Tree::Node*);
    Tree::Node* pop();
private:
	const uint8_t MaxSize;
	uint8_t Size;
    Tree::Node* arr[32];
    
};

#endif // DATASTRUCTURES

/**
* @file data_structures.hpp
* @brief «аголовочный файл с структурами данных
*/

#ifndef __DATASTRUCTURES
#define __DATASTRUCTURES

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned int uint32_t;

/**
* @brief ƒерево посто€нного указанного заранее размера - 32 узла
*/
class Tree
{
public:
    /**
    * @brief ”зел дерева
    */
    struct Node
    {
        Node() : Value(0), ChildsCount(0), Parent(nullptr) {}
        uint8_t Value;
        uint8_t ChildsCount;
        Node* ChildsArr[3];
        Node* Parent;
    };
	// Some constans
	enum
	{
		TREE_SIZE = 32,
	};

    Tree() : Size(0), MaxSize(TREE_SIZE) {}					// конструктор без аргументов
    Node* AddNodeLower(const uint8_t value);				// добавить операнд (узел не имеющий потомков)
    Node* AddNodeParent(uint8_t, Node**, int8_t);           // добавить оператор (аргументы: байт вместе с потомками)
    
	Node* GetBaseNode();									// получить указатель на самый верхний узел
    uint8_t GetSize() const;								// получить текущий размер дерева
    uint8_t GetMaxSize() const;								// получить максимальный размер дерева
private:
    uint8_t Size;
    const uint8_t MaxSize;
    Node ArrOfNodes[TREE_SIZE];
};


/**
* @brief —тек заранее заданного размера - 32 узла
*/
class Stack
{
public:
	// Some constans
	enum
	{
		STACK_SIZE = 32,
	};
    Stack() : Size(0), MaxSize(STACK_SIZE) {}
    void Push(Tree::Node* const  ptrNode);
    Tree::Node* Pop();
private:
    uint8_t Size;
    const uint8_t MaxSize;
    Tree::Node* Arr[STACK_SIZE];

};

#endif // __DATASTRUCTURES

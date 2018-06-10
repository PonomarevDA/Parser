#ifndef __DATASTRUCTURES
#define __DATASTRUCTURES

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned int uint32_t;

/*
* @brief Дерево постоянного указанного заранее размера
*/
class Tree
{
public:
    /*
    * @brief Узел дерева: хранит байт данных, кол-во потомков, указатель указателей на них и указатель на родителя
    */
    struct Node
    {
        Node() : Value(0), ChildsCount(0), ChildsArr(nullptr), Parent(nullptr) {}
        uint8_t Value;
        uint8_t ChildsCount;
        Node** ChildsArr;
        Node* Parent;
    };

    Tree() : Size(0), MaxSize(10) {}						// конструктор без аргументов - размер 32
    Tree(uint8_t maxSize) : Size(0), MaxSize(maxSize) {}	/*TODO: конструктор с аргументом, но размер все равно 32*/

    Node* AddNodeLower(uint8_t);							// добавить операнд (узел не имеет потомков)
    Node* AddNodeParent(uint8_t, Node**, uint8_t);          // добавить оператор (аргументы: байт вместе с потомками)
    Node* GetBaseNode();									// получить указатель на самый верхний узел

    uint8_t GetSize();
    uint8_t GetMaxSize();
private:
    uint8_t Size;
    const uint8_t MaxSize;
    Node ArrOfNodes[32];
};


/*
* @brief Стек заранее заданного размера - 32 байта
*/
class Stack
{
public:
    Stack() : Size(0), MaxSize(32) {}
    void Push(Tree::Node*);
    Tree::Node* Pop();
private:
    uint8_t Size;
    const uint8_t MaxSize;
    Tree::Node* Arr[32];

};

#endif // __DATASTRUCTURES

#ifndef __DATASTRUCTURES
#define __DATASTRUCTURES

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned int uint32_t;

/*
* @brief ������ ����������� ���������� ������� �������
*/
class Tree
{
public:
    /*
    * @brief ���� ������: ������ ���� ������, ���-�� ��������, ��������� ���������� �� ��� � ��������� �� ��������
    */
    struct Node
    {
        Node() : Value(0), ChildsCount(0), ChildsArr(nullptr), Parent(nullptr) {}
        uint8_t Value;
        uint8_t ChildsCount;
        Node** ChildsArr;
        Node* Parent;
    };

    Tree() : Size(0), MaxSize(10) {}						// ����������� ��� ���������� - ������ 32
    Tree(uint8_t maxSize) : Size(0), MaxSize(maxSize) {}	/*TODO: ����������� � ����������, �� ������ ��� ����� 32*/

    Node* AddNodeLower(uint8_t);							// �������� ������� (���� �� ����� ��������)
    Node* AddNodeParent(uint8_t, Node**, uint8_t);          // �������� �������� (���������: ���� ������ � ���������)
    Node* GetBaseNode();									// �������� ��������� �� ����� ������� ����

    uint8_t GetSize();
    uint8_t GetMaxSize();
private:
    uint8_t Size;
    const uint8_t MaxSize;
    Node ArrOfNodes[32];
};


/*
* @brief ���� ������� ��������� ������� - 32 �����
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

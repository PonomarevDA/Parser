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

    Tree() : Size(0), MaxSize(10) {}						// ����������� ��� ���������� - ������ 10
    Tree(uint8_t maxSize) : Size(0), MaxSize(maxSize) {}	// ����������� � ���������� - �������� ������

    Node* Add_node_lower(uint8_t);							// �������� ������� (���� �� ����� ��������)
    Node* Add_node_parent(uint8_t, Node**, uint8_t);		// �������� �������� (���������: ���� ������ � ���������)
    Node* Get_base_node();									// �������� ��������� �� ����� ������� ����
	void Do_reverse_calculate(uint32_t value);

    uint8_t Get_size();
    uint8_t Get_max_size();
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
	const uint8_t MaxSize;
	uint8_t Size;
    Tree::Node* Arr[32];
    
};

#endif // __DATASTRUCTURES

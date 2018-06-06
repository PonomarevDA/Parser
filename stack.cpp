/*
* @File Файл с реализацией методов класса Stack
*/
#include "stdafx.h"
#include "data_structures.hpp"
#include "obd.hpp"


void Stack::push(Tree::Node* node)
{
	arr[size++] = node;
}

Tree::Node* Stack::pop()
{
	return arr[--size];
}
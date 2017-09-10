#pragma once
#include <iostream>
#include <QString>

enum Status { MY_OVERFLOW = -1, ERROR, OK };
enum MYTYPE { NAME = 0, AREA, PRISCH, JUNSHC, HIGHSCH, UNIV, COM };
enum REC_TYPE { R_NAME = 0, R_AREA, R_SCH,  R_COM ,R_LIKE,R_ALL};

//Person类固定的变量(除链表和姓名外的变量)数量
const int VARIA_NUM = 6;

template <typename elem_type>
struct Node
{
    Node *next;
    elem_type elem;
    Node()
    {
    }
    Node(const Node &n)
    {
        this->elem = n.elem;
    }
};
//用来表示边的结构
struct edge
{
    int dest;
    //int val;
    bool operator==(const edge &e)const
    {
        return dest == e.dest;
    }
};
QString iToQString(int i);

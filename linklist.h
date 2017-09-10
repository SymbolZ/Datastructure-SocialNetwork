/*1552163 赵新博*/
#pragma once

#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <typeinfo>
#include "def.h"
using namespace std;


//若修改使用的数据类型，需要修改此函数
template <typename elem_type>
int elem_cmp(elem_type &A, elem_type &B)
{
    if (A > B)
        return 1;
    else if (A < B)
        return -1;
    return 0;
}
//直接输出A
template <typename elem_type>
void visit(elem_type A)
{
    cout << A.dest;
}
template <typename elem_type>
class LinkList
{
private:
    Node<elem_type>* head;
    Node<elem_type>* tail;
    int length_LL;

public:
    int indegree;
    int indegree_backup;

    LinkList();
    LinkList(const LinkList& l);

    LinkList(int n, elem_type elem);//初始化n个值为elem_type
    ~LinkList();

    Status ListInitial(int n);//根据用户输入，初始化长度为n的链表

    Status IniFirst(elem_type elem);//将elem插入到第一个节点之前
    Status DeFirst(elem_type &elem);//删除第一个节点，并返回新的第一个节点

    Status Append_elem(elem_type *elem);//将元素插入到表尾
    Status Append_elem(elem_type elem);//将元素插入到表尾
    Status Append_list(Node<elem_type>* p);//将指针指向的节点插入到表尾
    Status ListDelete_Sq_once(elem_type target);//删除表中第一个与target相等元素
    Status ListDelete_Sq_all(elem_type target);//删除表中所有与target相等元素
    Status GetElem(int position, elem_type *e);//获得指定位置的元素
    Status Remove_dul();//去掉表中所有重复元素
    Status invert();//逆转表中元素

    //入度处理
    Status IncreaseIndegree();
    Status DecreaseIndegree();
    int GetIndegree();
    int GetIndegreeBack();

    LinkList<elem_type>* UnionList(LinkList &l);
    bool Exist(elem_type e);

    Status ClearList();
    int ListEmpty();
    int ListLength();
    int LocateElem(elem_type e);//返回e在链表中的位置,compare函数elem_cmp已在上面定义
    Status ListInsert(int i, elem_type e);//在第i个元素前插入数据域为e的节点
    Status ListDelete(int i, elem_type &e);//删除第i个节点
    Status Pop();
    //Status PriorElem(elem_type cur_e,elem_type &e);
    //Status NextElem(elem_type cur_e, elem_type &e);
    Status ListTraverse();
    Status LocatePos(int i, Node<elem_type>* p);
    Status MergeList(LinkList &l);
    Node<elem_type>* Get_head() const;
    LinkList<elem_type> & operator=(const LinkList &l);
};

template<typename elem_type>
Node<elem_type>* LinkList<elem_type>::Get_head ()const
{
    return head;
}
template<typename elem_type>
inline LinkList<elem_type>& LinkList<elem_type>::operator=(const LinkList & l)
{
    this.c = l.c;
    this.indegree = l.indegree;
    this->indegree_backup = l.indegree_backup;
    Node<elem_type>* p = l.Get_head();

    while (p->next)
    {
        p = p->next;
        this.Append_elem(*(p->next));
    }
}
template<typename elem_type>
LinkList<elem_type>::LinkList()
{
    head = new Node<elem_type>;
    head->next = NULL;
    length_LL = 0;
    indegree = 0;
    indegree_backup = 0;
    tail = head;
}

template<typename elem_type>
LinkList<elem_type>::LinkList(const LinkList& l)
{
    head = new Node<elem_type>;
    head->next = NULL;
    length_LL = 0;
    indegree = 0;
    indegree_backup = 0;
    tail = head;

    this->indegree = l.indegree;
    this->indegree_backup = l.indegree_backup;

    Node<elem_type>* p = l.Get_head();

    while (p->next)
    {
        p = p->next;
        this->Append_elem(p->elem);
    }


}

template<typename elem_type>
LinkList<elem_type>::LinkList(int n, elem_type elem)
{
    //n个相同元素elem
    indegree = 0;
    indegree_backup = 0;
    Node<elem_type> *p, *q;
    head = new Node<elem_type>;
    head->next = NULL;
    p = head;
    for (int i = 0; i < n; ++i)
    {
        q = new Node<elem_type>;
        if (!q)
            exit(MY_OVERFLOW);
        q->elem = elem;
        p->next = q;
        p = p->next;
    }
    p->next = NULL;
    tail = p;
    length_LL = n;
}

template<typename elem_type>
LinkList<elem_type>::~LinkList()
{
    cout << "链表析构调用" << endl;
    Node<elem_type> *p;
    while (head)
    {
        p = head->next;
        delete head;
        head = p;
    }
}

template<typename elem_type>
Status LinkList<elem_type>::ListInitial(int n)
{
    if (n < 1)
        exit(ERROR);
    cout << "请连续输入" << n << "个整数,以空格或回车间隔" << endl;
    elem_type input;
    for (int i = 0; i < n; ++i)
    {
        //重要：若为其它自定义类型,需要重载cin>>操作符
        cin >> input;
        Append_elem(input);
    }
    return OK;

}

template<typename elem_type>
Status LinkList<elem_type>::IniFirst(elem_type elem)
{
    Node<elem_type> *p = new Node<elem_type>;
    if (!p)
        exit(MY_OVERFLOW);

    p->elem = elem;
    p->next = head->next;
    head->next = p;
    if (!length_LL)
        tail = p;
    ++length_LL;
    return OK;

}

template<typename elem_type>
Status LinkList<elem_type>::DeFirst(elem_type & elem)
{
    //或者写成
    //if(!head->next)
    if (!length_LL)
        return ERROR;
    Node<elem_type> *p = head->next;
    head->next = head->next->next;
    elem = p->elem;
    delete p;
    if (length_LL == 1)
        tail = head;
    --length_LL;
    return OK;
}

template<typename elem_type>
Status LinkList<elem_type>::Append_elem(elem_type elem)
{
    Node<elem_type> *p = new Node<elem_type>;
    if (!p)
        exit(MY_OVERFLOW);
    p->elem = elem;
    tail->next = p;
    p->next = NULL;
    tail = p;
    ++length_LL;
    return OK;
}


template<typename elem_type>
Status LinkList<elem_type>::Append_elem(elem_type *elem)
{
    Node<elem_type> *p = new Node<elem_type>;
    if (!p)
        exit(MY_OVERFLOW);
    p->elem = *elem;
    tail->next = p;
    p->next = NULL;
    tail = p;
    ++length_LL;
    return OK;
}

template<typename elem_type>
Status LinkList<elem_type>::ListDelete_Sq_once(elem_type target)
{
    Node<elem_type> *p = head, *q;
    while (p->next)
    {
        q = p;
        p = p->next;
        if (!elem_cmp(target, p->elem))
        {
            q->next = p->next;
            delete p;
            return OK;
        }
    }
    return ERROR;
}

template<typename elem_type>
Status LinkList<elem_type>::ListDelete_Sq_all(elem_type target)
{
    Node<elem_type> *p = head, *q;
    while (p->next)
    {
        q = p;
        p = p->next;
        if (!elem_cmp(target, p->elem))
        {
            q->next = p->next;
            delete p;
            p = q;
        }
    }
    return OK;
}

template<typename elem_type>
Status LinkList<elem_type>::ClearList()
{

    Node<elem_type> *p, *q;
    p = head->next;
    while (p)
    {
        //if(typeid(p->elem) == typeid(int))
        //    visit(p->elem);
        q = p->next;
        delete p;
        p = q;
    }
    head->next = NULL;
    length_LL = 0;
    return OK;
}

template<typename elem_type>
int LinkList<elem_type>::ListEmpty()
{
    return head->next ? 1 : 0;
}

template<typename elem_type>
int LinkList<elem_type>::ListLength()
{
     return length_LL;
    //添加private变量length_LL后，原有计算长度代码废弃
    /*int i = -1;
    Node<elem_type> *p = head;
    while (p)
    {
    p = p->next;
    ++i;
    }
    return i;*/
}

//输入一个链表，判断是否为循环表
template <typename elem_type>
int is_cycle_list(Node<elem_type> *head)
{
    Node<elem_type> *p = head;
    while (head->next)
    {
        head = head->next;
        if (head == p)
            return 1;
    }
    return 0;
}

template<typename elem_type>
Status LinkList<elem_type>::GetElem(int position, elem_type * e)
{
    //j等于1，同时从第一个节点开始循环，才能进行后面的条件判断
    int j = 0;
    Node<elem_type> *p = head->next;
    for (; j < position && p; ++j)
        p = p->next;

    if (!p || j>position)
        return ERROR;

    *e = p->elem;
    return OK;
}


template<typename elem_type>
Status LinkList<elem_type>::Remove_dul()
{
    Node <elem_type> *p = head->next, *q, *r;
    while (p)
    {
        q = p;
        while (q->next)
        {
            r = q;
            q = q->next;
            if (!elem_cmp(q->elem, p->elem))
            {
                r->next = q->next;
                delete q;
                q = r;
            }
        }
        p = p->next;
    }
    return OK;
}

template<typename elem_type>
Status LinkList<elem_type>::invert()
{
    elem_type *p = new elem_type[length_LL];
    int i = 0;
    Node<elem_type> *q = head->next;
    while (q)
    {
        p[i++] = q->elem;
        q = q->next;
    }

    --i;//i指向数组末端之后，需要使其指向末端
    q = head->next;
    while (q)
    {
        q->elem = p[i--];
        q = q->next;
    }
    delete p;
    return OK;
}

template<typename elem_type>
int LinkList<elem_type>::LocateElem(elem_type e)
{
    Node<elem_type> *p = head;
    int pos = 0;
    while (p->next)
    {
        p = p->next;
        ++pos;
        if (!elem_cmp(p->elem, e))
            break;
    }
    if (!p)
        return 0;
    return pos;
}

template<typename elem_type>
Status LinkList<elem_type>::ListInsert(int i, elem_type e)
{
    Node<elem_type> *p = head, *q;
    int j = 0;
    while (p && j < i - 1)
    {
        p = p->next;
        ++j;
    }
    //若i小于1,则j>i-1,处理了i<1的错误输入
    if (!p || j>i - 1)
        return ERROR;
    q = new Node<elem_type>;
    q->elem = e;
    q->next = p->next;
    p->next = q;
    //若插入在最后一的位置，更新尾节点
    if (i == length_LL)
        tail = q;
    return OK;

}

template<typename elem_type>
Status LinkList<elem_type>::ListDelete(int i, elem_type & e)
{
    if (i > length_LL || i<0)
        return ERROR;
    Node<elem_type> *p = head, *q;
    int j = 0;
    while (p && j < i)
    {
        p = p->next;
        ++j;
    }

    q = p->next;
    e = q->elem;
    //判别q的位置
    //若q在最后一位,避免非法访问
    p->next = q->next;

    delete q;
    //若删除在最后一的位置，更新尾节点
    if (i == length_LL)
        tail = p;
    --length_LL;
    return OK;
}

template<typename elem_type>
Status LinkList<elem_type>::Pop()
{
    elem_type e;
    return ListDelete(length_LL, e);
}


template<typename elem_type>
Status LinkList<elem_type>::ListTraverse()
{
    Node<elem_type> *p = head;
    while (p->next)
    {
        p = p->next;
        visit(p->elem);
        cout << " ";
    }
    cout << endl;
    return OK;
}

template<typename elem_type>
Status LinkList<elem_type>::LocatePos(int i, Node<elem_type>* p)
{
    if (i<1 || i>length_LL)
        return ERROR;
    p = head;
    for (int j = 0; j < i; ++j)
        p = p->next;
    return OK;
}

template<typename elem_type>
Status LinkList<elem_type>::MergeList(LinkList<elem_type> &l)
{
    tail->next = l.head->next;
    tail = l.tail;
    l.head->next = NULL;
    l.ClearList();
    return OK;
}

template<typename elem_type>
Status LinkList<elem_type>::IncreaseIndegree()
{
    ++indegree_backup;
    ++indegree;
    return OK;
}
template<typename elem_type>
Status LinkList<elem_type>::DecreaseIndegree()
{
    //--indegree_backup;
    --indegree;
    return OK;
}

template<typename elem_type>
int LinkList<elem_type>::GetIndegree()
{
    return indegree;
}

template<typename elem_type>
int LinkList<elem_type>::GetIndegreeBack()
{
    return indegree_backup;
}

template<typename elem_type>
LinkList<elem_type>* LinkList<elem_type>::UnionList(LinkList & l)
{
    //创建新表new list
    LinkList<elem_type>* nl = new LinkList<elem_type>;

    elem_type e;
    for (int i = 0; i < l.ListLength(); ++i)
    {
        l.GetElem(i, &e);
        if (!this->Exist(e))
            nl->Append_elem(e);
    }

    for (int i = 0; i < this->length_LL; ++i)
    {
        this->GetElem(i, &e);
        nl->Append_elem(e);
    }

    return nl;
}

template<typename elem_type>
inline bool LinkList<elem_type>::Exist(elem_type e)
{
    elem_type e2;
    for (int i = 0; i < this->length_LL; ++i)
    {
        this->GetElem(i, &e2);
        if (e2 == e)
            return true;
    }
    return false;
}

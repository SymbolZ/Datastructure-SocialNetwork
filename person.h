#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "linklist.h"
#include "def.h"
#include <string>

using namespace std;


class Person
{
private:
    LinkList<string>* hobby;//兴趣爱好、参加团体列表
    LinkList<edge>* friends;//朋友列表
public:

    void AddHobby(string hob);
    LinkList<string>* GetHobbyList();


    void AddFriend(int n);
    LinkList<edge>* GetFriendsList();

    int flag;//DFS标记
    int commonfriends;

    double value;//相似度
    int number;//编号
    string name;//姓名
    string area;//地区
    string priSch;//小学
    string junSch;//初中
    string highSch;//高中
    string univ;//大学
    string com;//公司
    Person();
    Person(string n, string a, string pri, string jun, string high, string univ, string co);

    ~Person();

};

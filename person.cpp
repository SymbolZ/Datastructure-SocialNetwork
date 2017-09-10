#include "person.h"

void Person::AddHobby(string hob)
{
    hobby->Append_elem(hob);
}

LinkList<string>* Person::GetHobbyList()
{
    return hobby;
}

void Person::AddFriend(int n)
{
    edge e;
    e.dest = n;
    friends->Append_elem(e);
}

LinkList<edge>* Person::GetFriendsList()
{
    return friends;
}

Person::Person()
{
    hobby = new LinkList<string>;
    friends = new LinkList<edge>;
}

Person::Person(string n, string a, string pri, string jun, string high, string univ, string co) :
    name(n), area(a),
    priSch(pri), junSch(jun),
    highSch(high), univ(univ),
    com(co)
{
    hobby = new LinkList<string>;
    friends = new LinkList<edge>;
}

Person::~Person()
{
    delete hobby;
    delete friends;
}

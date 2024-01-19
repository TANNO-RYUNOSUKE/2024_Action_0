#include "main.h"
#include <list>
#include <iterator>

#ifndef _List_H_
#define _List_H_


template <typename T> class Clist
{
public:
	void Regist(T data) { list.push_back(data); }
	void Delete(int nID) { list.erase(std::next(list.begin(), nID)); }
	void Clear() { list.clear(); }
	std::list<T> GetList() { return list; }

private:
	std::list<T> list;
};

#endif // !_List_H_
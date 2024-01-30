#include "main.h"
#include <list>
#include <iterator>

#ifndef _List_H_
#define _List_H_


template <typename T> class Clist
{
public:
	Clist() { list = new std::list<T>; }
	~Clist() { delete list; list = NULL; };
	void Regist(T data) { list->push_back(data); }
	void Delete(T data) { if (!list->empty()) { list->erase(std::find(list->begin(), list->end(), data)); } }
	T Find(T data) { if (!list->empty()) { return *std::find(list->begin(), list->end(), data); } else { return NULL; } }
	void Clear() { list->clear(); }
	
	std::list<T> * GetList() { return list; }
	int GetNum() {return (int)list->size(); }
	T Get(int nID) { return *std::next(list->begin(), nID); }
private:
	std::list<T> * list;
};


#endif // !_List_H_


#ifndef _EASYEDITOR_OBJECT_VECTOR_H_
#define _EASYEDITOR_OBJECT_VECTOR_H_

#include "DataTraverseType.h"

#include <vector>

namespace ee
{

class Visitor;

template<class T>
class ObjectVector
{
public:
	ObjectVector();
	virtual ~ObjectVector();
	
	void Traverse(Visitor& visitor, bool order = true) const;
	void Traverse(Visitor& visitor, DataTraverseType type = DT_ALL, bool order = true) const;

	bool Remove(T* obj);

	bool Insert(T* obj);
	bool Insert(T* obj, int idx);

	bool Clear();

	bool ResetOrder(const T* obj, bool up);
	bool ResetOrderMost(const T* obj, bool up);

	bool IsExist(T* obj) const;

	int Size() const;

public:
	static void Traverse(const std::vector<T*>& objs, Visitor& visitor, bool order = true);
	static void Traverse(const std::vector<T*>& objs, Visitor& visitor, DataTraverseType type = DT_ALL, bool order = true);

	static bool Remove(std::vector<T*>& objs, T* obj);

	static bool Insert(std::vector<T*>& objs, T* obj);
	static bool Insert(std::vector<T*>& objs, T* obj, int idx);

	static bool Clear(std::vector<T*>& objs);

	static bool ResetOrder(std::vector<T*>& objs, const T* obj, bool up);
	static bool ResetOrderMost(std::vector<T*>& objs, const T* obj, bool up);

private:
	std::vector<T*> m_objs;

}; // ObjectVector

}

#include "ObjectVector.inl"

#endif // _EASYEDITOR_OBJECT_VECTOR_H_
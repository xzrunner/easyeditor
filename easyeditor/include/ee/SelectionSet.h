#ifndef _EASYEDITOR_SELECTION_SET_H_
#define _EASYEDITOR_SELECTION_SET_H_

#include "Object.h"

#include <SM_Vector.h>

#include <vector>

namespace ee
{

class Visitor;

template<class T>
class SelectionSet : public Object
{
public:
	virtual ~SelectionSet() {}

	virtual void Clear();

	virtual void Add(T* item);
	virtual void Remove(T* item);

	size_t Size() const;
	bool IsEmpty() const;

	bool IsExist(T* item) const;

	void Traverse(Visitor& visitor) const;

protected:
	std::vector<T*> m_items;

}; // SelectionSet

typedef SelectionSet<sm::vec2> NodeSelection;

}

#include "SelectionSet.inl"

#endif // _EASYEDITOR_SELECTION_SET_H_
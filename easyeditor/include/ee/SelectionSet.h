#ifndef _EASYEDITOR_SELECTION_SET_H_
#define _EASYEDITOR_SELECTION_SET_H_

#include "Visitor.h"

#include <SM_Vector.h>
#include <CU_RefCountObj.h>

#include <vector>

namespace ee
{

template<class T>
class SelectionSet : public cu::RefCountObj
{
public:
	virtual ~SelectionSet() {}

	virtual void Clear();

	virtual void Add(T* item);
	virtual void Remove(T* item);

	size_t Size() const;
	bool IsEmpty() const;

	bool IsExist(T* item) const;

	void Traverse(Visitor<T>& visitor) const;

protected:
	std::vector<T*> m_items;

}; // SelectionSet

typedef SelectionSet<sm::vec2> NodeSelection;

}

#include "SelectionSet.inl"

#endif // _EASYEDITOR_SELECTION_SET_H_
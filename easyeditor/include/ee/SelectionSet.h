#ifndef _EASYEDITOR_SELECTION_SET_H_
#define _EASYEDITOR_SELECTION_SET_H_

#include "Visitor.h"

#include <SM_Vector.h>
#include <cu/CU_RefCountObj.h>

#include <vector>

namespace ee
{

template<class T>
class SelectionSet : public cu::RefCountObj
{
public:
	virtual ~SelectionSet() {}

	virtual void Clear();

	virtual void Add(const std::shared_ptr<T>& item);
	virtual void Remove(const std::shared_ptr<T>& item);

	size_t Size() const;
	bool IsEmpty() const;

	bool IsExist(const std::shared_ptr<T>& item) const;

	void Traverse(RefVisitor<T>& visitor) const;

protected:
	std::vector<std::shared_ptr<T>> m_items;

}; // SelectionSet

typedef SelectionSet<sm::vec2> NodeSelection;

}

#include "SelectionSet.inl"

#endif // _EASYEDITOR_SELECTION_SET_H_
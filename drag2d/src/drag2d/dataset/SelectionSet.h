#ifndef _D2D_SELECTION_SET_H_
#define _D2D_SELECTION_SET_H_

#include <vector>

#include "common/Vector.h"
#include "common/Object.h"

namespace d2d
{
class IVisitor;

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

	void Traverse(IVisitor& visitor) const;

protected:
	std::vector<T*> m_items;

}; // SelectionSet

typedef SelectionSet<Vector> NodeSelection;

}

#include "SelectionSet.inl"

#endif // _D2D_SELECTION_SET_H_
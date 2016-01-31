#ifndef _EASYEDITOR_OBJ_SELECTION_SET_H_
#define _EASYEDITOR_OBJ_SELECTION_SET_H_

#include "SelectionSet.h"

namespace ee
{

template<class T>
class ObjSelectionSet : public SelectionSet<T>
{
public:
	virtual ~ObjSelectionSet();

	virtual void Clear();

	virtual void Add(T* item);
	virtual void Remove(T* item);

}; // ObjSelectionSet

}

#include "ObjSelectionSet.inl"

#endif // _EASYEDITOR_OBJ_SELECTION_SET_H_
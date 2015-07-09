#ifndef _D2D_OBJ_SELECTION_SET_H_
#define _D2D_OBJ_SELECTION_SET_H_

#include "SelectionSet.h"

#include "dataset/ISprite.h"
#include "dataset/IShape.h"

namespace d2d
{

template<class T>
class ObjSelectionSet : public SelectionSet<T>
{
public:
	virtual ~ObjSelectionSet();

	virtual void Clear();

	virtual void Add(T* item);
	virtual void Remove(T* item);

}; // ObjSelection

// typedef ObjSelectionSet<ISprite> SpriteSelection;
// typedef ObjSelectionSet<IShape> ShapeSelection;

}

#include "ObjSelectionSet.inl"

#endif // _D2D_OBJ_SELECTION_SET_H_
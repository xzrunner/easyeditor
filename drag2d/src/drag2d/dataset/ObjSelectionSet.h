#ifndef _D2D_OBJ_SELECTION_SET_H_
#define _D2D_OBJ_SELECTION_SET_H_

#include "SelectionSet.h"

#include "dataset/Sprite.h"
#include "dataset/Shape.h"

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

// typedef ObjSelectionSet<Sprite> SpriteSelection;
// typedef ObjSelectionSet<Shape> ShapeSelection;

}

#include "ObjSelectionSet.inl"

#endif // _D2D_OBJ_SELECTION_SET_H_
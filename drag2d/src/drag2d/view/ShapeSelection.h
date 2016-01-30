#ifndef _DRAG2D_SHAPE_SELECTION_H_
#define _DRAG2D_SHAPE_SELECTION_H_

#include "dataset/Shape.h"
#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class IStageCanvas;

class ShapeSelection : public ObjSelectionSet<Shape>
{
public:
	virtual void Clear();

	virtual void Add(Shape* item);
	virtual void Remove(Shape* item);

}; // ShapeSelection

}

#endif // _DRAG2D_SHAPE_SELECTION_H_
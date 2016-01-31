#ifndef _EASYEDITOR_SHAPE_SELECTION_H_
#define _EASYEDITOR_SHAPE_SELECTION_H_

#include "Shape.h"
#include "ObjSelectionSet.h"

namespace ee
{

class ShapeSelection : public ObjSelectionSet<Shape>
{
public:
	virtual void Clear();

	virtual void Add(Shape* item);
	virtual void Remove(Shape* item);

}; // ShapeSelection

}

#endif // _EASYEDITOR_SHAPE_SELECTION_H_
#ifndef _EASYEDITOR_SHAPE_SELECTION_H_
#define _EASYEDITOR_SHAPE_SELECTION_H_

#include "Shape.h"
#include "SelectionSet.h"

namespace ee
{

class ShapeSelection : public SelectionSet<Shape>
{
public:
	virtual void Clear();

	virtual void Add(const ShapePtr& item);
	virtual void Remove(const ShapePtr& item);

}; // ShapeSelection

}

#endif // _EASYEDITOR_SHAPE_SELECTION_H_
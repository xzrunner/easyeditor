#ifndef _EASYEDITOR_SHAPES_CONTAINER_H_
#define _EASYEDITOR_SHAPES_CONTAINER_H_

#include "DataContainer.h"
#include "ObjectVector.h"

namespace ee
{

class Shape;

class ShapesContainer : public DataContainer<ee::Shape>
{
public:
	virtual ~ShapesContainer();

	//
	// DataContainer interface
	//
	virtual void Traverse(Visitor<ee::Shape>& visitor, bool order = true) const;
	virtual void Traverse(Visitor<ee::Shape>& visitor, DataTraverseType type = DT_ALL, bool order = true) const;
	virtual bool Remove(ee::Shape* shape);
	virtual bool Insert(ee::Shape* shape);
	virtual bool Insert(ee::Shape* shape, int idx);
	virtual bool Clear();
	virtual bool ResetOrder(const ee::Shape* shape, bool up);
	virtual bool ResetOrderMost(const ee::Shape* shape, bool up);

private:
	ObjectVector<Shape> m_shapes;

}; // ShapesContainer

}

#endif // _EASYEDITOR_SHAPES_CONTAINER_H_
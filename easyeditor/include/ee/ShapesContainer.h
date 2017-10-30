#ifndef _EASYEDITOR_SHAPES_CONTAINER_H_
#define _EASYEDITOR_SHAPES_CONTAINER_H_

#include "DataContainer.h"
#include "ObjectVector.h"
#include "Shape.h"

namespace ee
{

class ShapesContainer : public DataContainer<ee::Shape>
{
public:
	virtual ~ShapesContainer();

	//
	// DataContainer interface
	//
	virtual void Traverse(RefVisitor<ee::Shape>& visitor, bool order = true) const;
	virtual void Traverse(RefVisitor<ee::Shape>& visitor, DataTraverseType type = DT_ALL, bool order = true) const;
	virtual bool Remove(const ee::ShapePtr& shape);
	virtual bool Insert(const ee::ShapePtr& shape);
	virtual bool Insert(const ee::ShapePtr& shape, int idx);
	virtual bool Clear();
	virtual bool ResetOrder(const ee::ShapeConstPtr& shape, bool up);
	virtual bool ResetOrderMost(const ee::ShapeConstPtr& shape, bool up);

private:
	ObjectVector<Shape> m_shapes;

}; // ShapesContainer

}

#endif // _EASYEDITOR_SHAPES_CONTAINER_H_
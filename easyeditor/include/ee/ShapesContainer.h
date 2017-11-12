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
	virtual void Traverse(RefVisitor<ee::Shape>& visitor, bool order = true) const override;
	virtual void Traverse(RefVisitor<ee::Shape>& visitor, DataTraverseType type = DT_ALL, bool order = true) const override;
	virtual bool Remove(const ee::ShapePtr& shape) override;
	virtual bool Insert(const ee::ShapePtr& shape) override;
	virtual bool Insert(const ee::ShapePtr& shape, int idx) override;
	virtual bool Clear() override;
	virtual bool ResetOrder(const ee::ShapeConstPtr& shape, bool up) override;
	virtual bool ResetOrderMost(const ee::ShapeConstPtr& shape, bool up) override;

private:
	ObjectVector<Shape> m_shapes;

}; // ShapesContainer

}

#endif // _EASYEDITOR_SHAPES_CONTAINER_H_
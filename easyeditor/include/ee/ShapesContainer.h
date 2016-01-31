#ifndef _EASYEDITOR_SHAPES_CONTAINER_H_
#define _EASYEDITOR_SHAPES_CONTAINER_H_

#include "DataContainer.h"
#include "ObjectVector.h"

namespace ee
{

class Shape;

class ShapesContainer : public DataContainer
{
public:
	virtual ~ShapesContainer();

	//
	// DataContainer interface
	//
	virtual void Traverse(Visitor& visitor, bool order = true) const;
	virtual void Traverse(Visitor& visitor, DataTraverseType type = DT_ALL, bool order = true) const;
	virtual bool Remove(Object* obj);
	virtual bool Insert(Object* obj);
	virtual bool Insert(Object* obj, int idx);
	virtual bool Clear();
	virtual bool ResetOrder(const Object* obj, bool up);
	virtual bool ResetOrderMost(const Object* obj, bool up);

private:
	ObjectVector<Shape> m_shapes;

}; // ShapesContainer

}

#endif // _EASYEDITOR_SHAPES_CONTAINER_H_
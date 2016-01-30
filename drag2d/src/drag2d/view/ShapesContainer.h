#ifndef _DRAG2D_SHAPES_CONTAINER_H_
#define _DRAG2D_SHAPES_CONTAINER_H_

#include "IDataContainer.h"
#include "ObjectVector.h"

namespace d2d
{

class Shape;

class ShapesContainer : public IDataContainer
{
public:
	virtual ~ShapesContainer();

	//
	// IDataContainer interface
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

#endif // _DRAG2D_SHAPES_CONTAINER_H_
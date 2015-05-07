#ifndef _DRAG2D_SHAPES_CONTAINER_H_
#define _DRAG2D_SHAPES_CONTAINER_H_

#include "IDataContainer.h"

namespace d2d
{

class IShape;

class ShapesContainer : public IDataContainer
{
public:
	virtual ~ShapesContainer();

	//
	// IDataContainer interface
	//
	virtual void traverse(IVisitor& visitor, bool order = true) const;
	virtual void traverse(IVisitor& visitor, TraverseType type = e_allExisting, bool order = true) const;
	virtual void remove(Object* obj);
	virtual void insert(Object* obj);
	virtual void clear();
	virtual void resetOrder(const Object* obj, bool up);

private:
	std::vector<IShape*> m_shapes;

}; // ShapesContainer

}

#endif // _DRAG2D_SHAPES_CONTAINER_H_
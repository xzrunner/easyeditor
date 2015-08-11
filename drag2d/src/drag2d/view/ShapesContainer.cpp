#include "ShapesContainer.h"
#include "ObjectVector.h"

#include "dataset/IShape.h"

namespace d2d
{

ShapesContainer::~ShapesContainer()
{
	Clear();
}

void ShapesContainer::Traverse(IVisitor& visitor, bool order/* = true*/) const
{
	m_shapes.Traverse(visitor, order);
}

void ShapesContainer::Traverse(IVisitor& visitor, DataTraverseType type, bool order) const
{
	Traverse(visitor, order);
}

bool ShapesContainer::Remove(Object* obj)
{
	return m_shapes.Remove(static_cast<IShape*>(obj));
}

bool ShapesContainer::Insert(Object* obj)
{
	return m_shapes.Insert(static_cast<IShape*>(obj));
}

bool ShapesContainer::Insert(Object* obj, int idx)
{
	return m_shapes.Insert(static_cast<IShape*>(obj), idx);	
}

bool ShapesContainer::Clear()
{
	return m_shapes.Clear();
}

bool ShapesContainer::ResetOrder(const Object* obj, bool up)
{
	return m_shapes.ResetOrder(static_cast<const IShape*>(obj), up);
}

}
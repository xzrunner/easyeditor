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

void ShapesContainer::Insert(Object* obj)
{
	m_shapes.Insert(static_cast<IShape*>(obj));
}

void ShapesContainer::Clear()
{
	m_shapes.Clear();
}

bool ShapesContainer::ResetOrder(const Object* obj, bool up)
{
	return m_shapes.ResetOrder(static_cast<const IShape*>(obj), up);
}

}
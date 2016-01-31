#include "ShapesContainer.h"
#include "ObjectVector.h"
#include "Shape.h"

namespace ee
{

ShapesContainer::~ShapesContainer()
{
	Clear();
}

void ShapesContainer::Traverse(Visitor& visitor, bool order/* = true*/) const
{
	m_shapes.Traverse(visitor, order);
}

void ShapesContainer::Traverse(Visitor& visitor, DataTraverseType type, bool order) const
{
	Traverse(visitor, order);
}

bool ShapesContainer::Remove(Object* obj)
{
	return m_shapes.Remove(static_cast<Shape*>(obj));
}

bool ShapesContainer::Insert(Object* obj)
{
	return m_shapes.Insert(static_cast<Shape*>(obj));
}

bool ShapesContainer::Insert(Object* obj, int idx)
{
	return m_shapes.Insert(static_cast<Shape*>(obj), idx);	
}

bool ShapesContainer::Clear()
{
	return m_shapes.Clear();
}

bool ShapesContainer::ResetOrder(const Object* obj, bool up)
{
	return m_shapes.ResetOrder(static_cast<const Shape*>(obj), up);
}

bool ShapesContainer::ResetOrderMost(const Object* obj, bool up)
{
	return m_shapes.ResetOrderMost(static_cast<const Shape*>(obj), up);
}

}
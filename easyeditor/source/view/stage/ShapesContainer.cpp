#include "ShapesContainer.h"
#include "ObjectVector.h"
#include "Shape.h"

namespace ee
{

ShapesContainer::~ShapesContainer()
{
	Clear();
}

void ShapesContainer::Traverse(Visitor<ee::Shape>& visitor, bool order/* = true*/) const
{
	m_shapes.Traverse(visitor, order);
}

void ShapesContainer::Traverse(Visitor<ee::Shape>& visitor, DataTraverseType type, bool order) const
{
	Traverse(visitor, order);
}

bool ShapesContainer::Remove(ee::Shape* shape)
{
	return m_shapes.Remove(shape);
}

bool ShapesContainer::Insert(ee::Shape* shape)
{
	return m_shapes.Insert(shape);
}

bool ShapesContainer::Insert(ee::Shape* shape, int idx)
{
	return m_shapes.Insert(shape, idx);	
}

bool ShapesContainer::Clear()
{
	return m_shapes.Clear();
}

bool ShapesContainer::ResetOrder(const ee::Shape* shape, bool up)
{
	return m_shapes.ResetOrder(shape, up);
}

bool ShapesContainer::ResetOrderMost(const ee::Shape* shape, bool up)
{
	return m_shapes.ResetOrderMost(shape, up);
}

}
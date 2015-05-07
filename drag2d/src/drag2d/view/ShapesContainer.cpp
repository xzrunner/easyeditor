#include "ShapesContainer.h"
#include "VectorContainer.h"

#include "dataset/IShape.h"

namespace d2d
{

ShapesContainer::~ShapesContainer()
{
	Clear();
}

void ShapesContainer::Traverse(IVisitor& visitor, bool order/* = true*/) const
{
	VectorContainer::Traverse(m_shapes, visitor, order);
}

void ShapesContainer::Traverse(IVisitor& visitor, DataTraverseType type, bool order) const
{
	Traverse(visitor, order);
}

bool ShapesContainer::Remove(Object* obj)
{
	return VectorContainer::Remove(m_shapes, static_cast<IShape*>(obj));
}

void ShapesContainer::Insert(Object* obj)
{
	VectorContainer::Insert(m_shapes, static_cast<IShape*>(obj));
}

void ShapesContainer::Clear()
{
	VectorContainer::Clear(m_shapes);
}

bool ShapesContainer::ResetOrder(const Object* obj, bool up)
{
	return VectorContainer::ResetOrder(m_shapes, static_cast<const IShape*>(obj), up);
}

}
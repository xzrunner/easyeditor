#include "ShapesContainer.h"
#include "VectorUtils.h"

#include "dataset/IShape.h"

namespace d2d
{

ShapesContainer::~ShapesContainer()
{
	clear();
}

void ShapesContainer::traverse(IVisitor& visitor, bool order/* = true*/) const
{
	VectorUtils::traverse(m_shapes, visitor, order);
}

void ShapesContainer::remove(Object* obj)
{
	VectorUtils::remove(m_shapes, static_cast<IShape*>(obj));
}

void ShapesContainer::insert(Object* obj)
{
	VectorUtils::insert(m_shapes, static_cast<IShape*>(obj));
}

void ShapesContainer::clear()
{
	VectorUtils::clear(m_shapes);
}

void ShapesContainer::resetOrder(const Object* obj, bool up)
{
	VectorUtils::resetOrder(m_shapes, static_cast<const IShape*>(obj), up);
}

}
#include "Layer.h"

namespace lr
{

Layer::Layer()
{
	
}

void Layer::traverse(d2d::IVisitor& visitor, bool order/* = true*/) const
{
	d2d::VectorUtils::traverse(m_sprs, visitor, order);
}

void Layer::traverse(d2d::IVisitor& visitor, d2d::TraverseType type, bool order) const
{
	d2d::VectorUtils::traverse(m_sprs, visitor, type, order);
}

void Layer::remove(Object* obj)
{
	d2d::VectorUtils::remove(m_sprs, static_cast<d2d::ISprite*>(obj));
}

void Layer::insert(Object* obj)
{
	d2d::VectorUtils::insert(m_sprs, static_cast<d2d::ISprite*>(obj));
}

void Layer::clear()
{
	d2d::VectorUtils::clear(m_sprs);
}

void Layer::resetOrder(const Object* obj, bool up)
{
	d2d::VectorUtils::resetOrder(m_sprs, static_cast<const d2d::ISprite*>(obj), up);
}


}
#include "Layer.h"

namespace lr
{

Layer::Layer()
	: m_editable(true)
	, m_visible(true)
{	
}

void Layer::Traverse(d2d::IVisitor& visitor, bool order/* = true*/) const
{
	d2d::VectorContainer::Traverse(m_sprs, visitor, order);
}

void Layer::Traverse(d2d::IVisitor& visitor, d2d::DataTraverseType type, bool order) const
{
	d2d::VectorContainer::Traverse(m_sprs, visitor, type, order);
}

bool Layer::Remove(Object* obj)
{
	return d2d::VectorContainer::Remove(m_sprs, static_cast<d2d::ISprite*>(obj));
}

void Layer::Insert(Object* obj)
{
	d2d::VectorContainer::Insert(m_sprs, static_cast<d2d::ISprite*>(obj));
}

void Layer::Clear()
{
	d2d::VectorContainer::Clear(m_sprs);
}

bool Layer::ResetOrder(const Object* obj, bool up)
{
	return d2d::VectorContainer::ResetOrder(m_sprs, static_cast<const d2d::ISprite*>(obj), up);
}


}
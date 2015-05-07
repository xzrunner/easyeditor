#include "SpritesContainer.h"
#include "VectorContainer.h"

#include "dataset/ISprite.h"

namespace d2d
{

SpritesContainer::~SpritesContainer()
{
	Clear();
}

void SpritesContainer::Traverse(IVisitor& visitor, bool order/* = true*/) const
{
	VectorContainer::Traverse(m_sprites, visitor, order);
}

void SpritesContainer::Traverse(IVisitor& visitor, DataTraverseType type, bool order) const
{
	VectorContainer::Traverse(m_sprites, visitor, type, order);
}

bool SpritesContainer::Remove(Object* obj)
{
	return VectorContainer::Remove(m_sprites, static_cast<ISprite*>(obj));
}

void SpritesContainer::Insert(Object* obj)
{
	VectorContainer::Insert(m_sprites, static_cast<ISprite*>(obj));
}

void SpritesContainer::Clear()
{
	VectorContainer::Clear(m_sprites);
}

bool SpritesContainer::ResetOrder(const Object* obj, bool up)
{
	return VectorContainer::ResetOrder(m_sprites, static_cast<const ISprite*>(obj), up);
}

}
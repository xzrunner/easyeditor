#include "SpritesContainer.h"
#include "ObjectVector.h"

#include "dataset/ISprite.h"

namespace d2d
{

SpritesContainer::~SpritesContainer()
{
	Clear();
}

void SpritesContainer::Traverse(IVisitor& visitor, bool order/* = true*/) const
{
	m_sprites.Traverse(visitor, order);
}

void SpritesContainer::Traverse(IVisitor& visitor, DataTraverseType type, bool order) const
{
	m_sprites.Traverse(visitor, type, order);
}

bool SpritesContainer::Remove(Object* obj)
{
	return m_sprites.Remove(static_cast<ISprite*>(obj));
}

void SpritesContainer::Insert(Object* obj)
{
	m_sprites.Insert(static_cast<ISprite*>(obj));
}

void SpritesContainer::Clear()
{
	m_sprites.Clear();
}

bool SpritesContainer::ResetOrder(const Object* obj, bool up)
{
	return m_sprites.ResetOrder(static_cast<const ISprite*>(obj), up);
}

}
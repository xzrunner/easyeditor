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

bool SpritesContainer::Insert(Object* obj)
{
	return m_sprites.Insert(static_cast<ISprite*>(obj));
}

bool SpritesContainer::Insert(Object* obj, int idx)
{
	return m_sprites.Insert(static_cast<ISprite*>(obj), idx);
}

bool SpritesContainer::Clear()
{
	return m_sprites.Clear();
}

bool SpritesContainer::ResetOrder(const Object* obj, bool up)
{
	return m_sprites.ResetOrder(static_cast<const ISprite*>(obj), up);
}

int SpritesContainer::Size() const
{
	return m_sprites.Size();
}

}
#include "SpritesContainer.h"
#include "ObjectVector.h"
#include "Sprite.h"

namespace ee
{

SpritesContainer::~SpritesContainer()
{
	Clear();
}

void SpritesContainer::Traverse(Visitor& visitor, bool order/* = true*/) const
{
	m_sprites.Traverse(visitor, order);
}

void SpritesContainer::Traverse(Visitor& visitor, DataTraverseType type, bool order) const
{
	m_sprites.Traverse(visitor, type, order);
}

bool SpritesContainer::Remove(Object* obj)
{
	return m_sprites.Remove(static_cast<Sprite*>(obj));
}

bool SpritesContainer::Insert(Object* obj)
{
	return m_sprites.Insert(static_cast<Sprite*>(obj));
}

bool SpritesContainer::Insert(Object* obj, int idx)
{
	return m_sprites.Insert(static_cast<Sprite*>(obj), idx);
}

bool SpritesContainer::Clear()
{
	return m_sprites.Clear();
}

bool SpritesContainer::ResetOrder(const Object* obj, bool up)
{
	return m_sprites.ResetOrder(static_cast<const Sprite*>(obj), up);
}

bool SpritesContainer::ResetOrderMost(const Object* obj, bool up)
{
	return m_sprites.ResetOrderMost(static_cast<const Sprite*>(obj), up);	
}

int SpritesContainer::Size() const
{
	return m_sprites.Size();
}

}
#include "SpritesContainer.h"
#include "ObjectVector.h"
#include "Sprite.h"

namespace ee
{

SpritesContainer::~SpritesContainer()
{
	Clear();
}

void SpritesContainer::Traverse(Visitor<Sprite>& visitor, bool order/* = true*/) const
{
	m_sprites.Traverse(visitor, order);
}

void SpritesContainer::Traverse(Visitor<Sprite>& visitor, DataTraverseType type, bool order) const
{
	m_sprites.Traverse(visitor, type, order);
}

bool SpritesContainer::Remove(Sprite* spr)
{
	return m_sprites.Remove(spr);
}

bool SpritesContainer::Insert(Sprite* spr)
{
	return m_sprites.Insert(spr);
}

bool SpritesContainer::Insert(Sprite* spr, int idx)
{
	return m_sprites.Insert(spr, idx);
}

bool SpritesContainer::Clear()
{
	return m_sprites.Clear();
}

bool SpritesContainer::ResetOrder(const Sprite* spr, bool up)
{
	return m_sprites.ResetOrder(spr, up);
}

bool SpritesContainer::ResetOrderMost(const Sprite* spr, bool up)
{
	return m_sprites.ResetOrderMost(spr, up);	
}

bool SpritesContainer::Sort(std::vector<Sprite*>& sprites)
{
	return m_sprites.Sort(sprites);
}

int SpritesContainer::Size() const
{
	return m_sprites.Size();
}

}
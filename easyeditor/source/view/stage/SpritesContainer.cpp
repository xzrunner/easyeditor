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
	m_sprs.Traverse(visitor, order);
}

void SpritesContainer::Traverse(Visitor<Sprite>& visitor, DataTraverseType type, bool order) const
{
	m_sprs.Traverse(visitor, type, order);
}

bool SpritesContainer::Remove(Sprite* spr)
{
	return m_sprs.Remove(spr);
}

bool SpritesContainer::Insert(Sprite* spr)
{
	return m_sprs.Insert(spr);
}

bool SpritesContainer::Insert(Sprite* spr, int idx)
{
	return m_sprs.Insert(spr, idx);
}

bool SpritesContainer::Clear()
{
	return m_sprs.Clear();
}

bool SpritesContainer::ResetOrder(const Sprite* spr, bool up)
{
	return m_sprs.ResetOrder(spr, up);
}

bool SpritesContainer::ResetOrderMost(const Sprite* spr, bool up)
{
	return m_sprs.ResetOrderMost(spr, up);	
}

bool SpritesContainer::Sort(std::vector<Sprite*>& sprs)
{
	return m_sprs.Sort(sprs);
}

int SpritesContainer::Size() const
{
	return m_sprs.Size();
}

}
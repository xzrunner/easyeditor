#include "SpritesContainer.h"
#include "ObjectVector.h"
#include "Sprite.h"

namespace ee
{

SpritesContainer::~SpritesContainer()
{
	Clear();
}

void SpritesContainer::Traverse(RefVisitor<Sprite>& visitor, bool order/* = true*/) const
{
	m_sprs.Traverse(visitor, order);
}

void SpritesContainer::Traverse(RefVisitor<Sprite>& visitor, DataTraverseType type, bool order) const
{
	m_sprs.Traverse(visitor, type, order);
}

bool SpritesContainer::Remove(const SprPtr& spr)
{
	return m_sprs.Remove(spr);
}

bool SpritesContainer::Insert(const SprPtr& spr)
{
	return m_sprs.Insert(spr);
}

bool SpritesContainer::Insert(const SprPtr& spr, int idx)
{
	return m_sprs.Insert(spr, idx);
}

bool SpritesContainer::Clear()
{
	return m_sprs.Clear();
}

bool SpritesContainer::ResetOrder(const SprConstPtr& spr, bool up)
{
	return m_sprs.ResetOrder(std::const_pointer_cast<Sprite>(spr), up);
}

bool SpritesContainer::ResetOrderMost(const SprConstPtr& spr, bool up)
{
	return m_sprs.ResetOrderMost(std::const_pointer_cast<Sprite>(spr), up);
}

bool SpritesContainer::Sort(std::vector<SprPtr>& sprs)
{
	return m_sprs.Sort(sprs);
}

int SpritesContainer::Size() const
{
	return m_sprs.Size();
}

}
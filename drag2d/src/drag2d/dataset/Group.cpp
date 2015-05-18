#include "Group.h"

namespace d2d
{

Group::Group(const std::string& name)
	: m_name(name)
{
}

Group::~Group()
{
	Clear();
}

void Group::TraverseSprite(IVisitor& visitor, DataTraverseType type, bool order) const
{
	m_sprites.Traverse(visitor, type, order);
}

bool Group::Insert(ISprite* sprite)
{
	if (m_sprites.IsExist(sprite)) {
		return false;
	} else {
		m_sprites.Insert(sprite);
		return true;
	}
}

bool Group::Remove(ISprite* sprite)
{
	return m_sprites.Remove(sprite);
}

void Group::Clear()
{
	m_sprites.Clear();
	m_shapes.Clear();
}

}
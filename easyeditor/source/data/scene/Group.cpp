#include "Group.h"

namespace ee
{

Group::Group(const std::string& name, bool visible, bool editable)
	: m_name(name)
	, m_visible(visible)
	, m_editable(editable)
{
}

Group::~Group()
{
	Clear();
}

void Group::TraverseSprite(Visitor<Sprite>& visitor, DataTraverseType type, bool order) const
{
	if (type == DT_EDITABLE && m_editable ||
		type == DT_VISIBLE && m_visible ||
		type == DT_ALL || type == DT_SELECTABLE)
	{
		m_sprites.Traverse(visitor, type, order);
	}
}

bool Group::Insert(Sprite* sprite)
{
	if (m_sprites.IsExist(sprite)) {
		return false;
	} else {
// 		sprite->visiable = m_visible;
// 		sprite->editable = m_editable;
		m_sprites.Insert(sprite);
		return true;
	}
}

bool Group::Remove(Sprite* sprite)
{
	return m_sprites.Remove(sprite);
}

void Group::Clear()
{
	m_sprites.Clear();
	m_shapes.Clear();
}

}
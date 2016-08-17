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
		m_sprs.Traverse(visitor, type, order);
	}
}

bool Group::Insert(Sprite* spr)
{
	if (m_sprs.IsExist(spr)) {
		return false;
	} else {
// 		spr->visiable = m_visible;
// 		spr->editable = m_editable;
		m_sprs.Insert(spr);
		return true;
	}
}

bool Group::Remove(Sprite* spr)
{
	return m_sprs.Remove(spr);
}

void Group::Clear()
{
	m_sprs.Clear();
	m_shapes.Clear();
}

}
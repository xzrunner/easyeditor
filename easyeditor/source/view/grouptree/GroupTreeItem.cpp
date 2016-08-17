#include "GroupTreeItem.h"
#include "Sprite.h"
#include "Group.h"

namespace ee
{

//////////////////////////////////////////////////////////////////////////
// class GroupTreeSpriteItem
//////////////////////////////////////////////////////////////////////////

GroupTreeSpriteItem::GroupTreeSpriteItem(Sprite* spr)
	: m_spr(spr)
{
	if (m_spr) {
		m_spr->AddReference();
	}
}

GroupTreeSpriteItem::GroupTreeSpriteItem(const GroupTreeSpriteItem& item)
{
	m_spr = item.m_spr;
	if (m_spr) {
		m_spr->AddReference();
	}
}

GroupTreeSpriteItem::~GroupTreeSpriteItem()
{
	if (m_spr) {
		m_spr->RemoveReference();
	}
}

//////////////////////////////////////////////////////////////////////////
// class GroupTreeGroupItem
//////////////////////////////////////////////////////////////////////////

GroupTreeGroupItem::GroupTreeGroupItem(Group* group)
	: m_group(group)
{
	if (m_group) {
		m_group->AddReference();
	}
}

GroupTreeGroupItem::GroupTreeGroupItem(const GroupTreeGroupItem& item)
{
	m_group = item.m_group;
	if (m_group) {
		m_group->AddReference();
	}
}

GroupTreeGroupItem::~GroupTreeGroupItem()
{
	if (m_group) {
		m_group->RemoveReference();
	}
}

}
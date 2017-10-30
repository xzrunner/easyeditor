#include "GroupTreeItem.h"
#include "Sprite.h"
#include "Group.h"

namespace ee
{

//////////////////////////////////////////////////////////////////////////
// class GroupTreeSpriteItem
//////////////////////////////////////////////////////////////////////////

GroupTreeSpriteItem::GroupTreeSpriteItem(const SprPtr& spr)
	: m_spr(spr)
{
}

GroupTreeSpriteItem::GroupTreeSpriteItem(const GroupTreeSpriteItem& item)
	: m_spr(item.m_spr)
{
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
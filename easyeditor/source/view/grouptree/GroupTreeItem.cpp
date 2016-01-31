#include "GroupTreeItem.h"
#include "Sprite.h"
#include "Group.h"

namespace ee
{

//////////////////////////////////////////////////////////////////////////
// class GroupTreeSpriteItem
//////////////////////////////////////////////////////////////////////////

GroupTreeSpriteItem::GroupTreeSpriteItem(Sprite* sprite)
	: m_sprite(sprite)
{
	if (m_sprite) {
		m_sprite->Retain();
	}
}

GroupTreeSpriteItem::GroupTreeSpriteItem(const GroupTreeSpriteItem& item)
{
	m_sprite = item.m_sprite;
	if (m_sprite) {
		m_sprite->Retain();
	}
}

GroupTreeSpriteItem::~GroupTreeSpriteItem()
{
	if (m_sprite) {
		m_sprite->Release();
	}
}

//////////////////////////////////////////////////////////////////////////
// class GroupTreeGroupItem
//////////////////////////////////////////////////////////////////////////

GroupTreeGroupItem::GroupTreeGroupItem(Group* group)
	: m_group(group)
{
	if (m_group) {
		m_group->Retain();
	}
}

GroupTreeGroupItem::GroupTreeGroupItem(const GroupTreeGroupItem& item)
{
	m_group = item.m_group;
	if (m_group) {
		m_group->Retain();
	}
}

GroupTreeGroupItem::~GroupTreeGroupItem()
{
	if (m_group) {
		m_group->Release();
	}
}

}
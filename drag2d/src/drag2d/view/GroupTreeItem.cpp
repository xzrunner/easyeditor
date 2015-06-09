#include "GroupTreeItem.h"

#include "dataset/ISprite.h"
#include "dataset/Group.h"

namespace d2d
{

GroupTreeItem::GroupTreeItem(Group* group)
	: m_group(group)
	, m_sprite(NULL)
{
	if (m_group) {
		m_group->Retain();
	}
}

GroupTreeItem::GroupTreeItem(ISprite* sprite)
	: m_sprite(sprite)
	, m_group(NULL)
{
	if (m_sprite) {
		m_sprite->Retain();
	}
}

GroupTreeItem::GroupTreeItem(const GroupTreeItem& item)
{
	m_group = item.m_group;
	if (m_group) {
		m_group->Retain();
	}

	m_sprite = item.m_sprite;
	if (m_sprite) {
		m_sprite->Retain();
	}
}

GroupTreeItem::~GroupTreeItem()
{
	if (m_group) {
		m_group->Release();
	}
	if (m_sprite) {
		m_sprite->Release();
	}
}

}
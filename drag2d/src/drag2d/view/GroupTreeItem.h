#ifndef _DRAG2D_GROUP_TREE_ITEM_H_
#define _DRAG2D_GROUP_TREE_ITEM_H_

#include <wx/treectrl.h>

namespace d2d
{

class Group;
class ISprite;

class GroupTreeItem : public wxTreeItemData
{
public:
	virtual ~GroupTreeItem() {}
	virtual GroupTreeItem* Clone() const  = 0;
	virtual bool IsGroup() const = 0;
}; // GroupTreeItem

class GroupTreeSpriteItem : public GroupTreeItem
{
public:
	GroupTreeSpriteItem(ISprite* sprite);
	GroupTreeSpriteItem(const GroupTreeSpriteItem& item);
	virtual ~GroupTreeSpriteItem();

	virtual GroupTreeItem* Clone() const { return new GroupTreeSpriteItem(*this); }

	virtual bool IsGroup() const { return false; }

	ISprite* GetSprite() { return m_sprite; }

private:
	ISprite* m_sprite;

}; // GroupTreeSpriteItem

class GroupTreeGroupItem : public GroupTreeItem
{
public:
	GroupTreeGroupItem(Group* group);
	GroupTreeGroupItem(const GroupTreeGroupItem& item);
	virtual ~GroupTreeGroupItem();

	virtual GroupTreeItem* Clone() const { return new GroupTreeGroupItem(*this); }

	virtual bool IsGroup() const { return true; }

	Group* GetGroup() { return m_group; }

private:
	Group* m_group;

}; // GroupTreeGroupItem

}

#endif // _DRAG2D_GROUP_TREE_ITEM_H_
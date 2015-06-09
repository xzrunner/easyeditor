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
	GroupTreeItem(Group* group);
	GroupTreeItem(ISprite* sprite);
	GroupTreeItem(const GroupTreeItem& item);
	virtual ~GroupTreeItem();

	GroupTreeItem* Clone() const { return new GroupTreeItem(*this); }

public:
	Group* m_group;

	ISprite* m_sprite;

}; // GroupTreeItem

}

#endif // _DRAG2D_GROUP_TREE_ITEM_H_
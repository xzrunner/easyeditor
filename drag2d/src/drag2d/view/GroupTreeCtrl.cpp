#include "GroupTreeCtrl.h"
#include "MultiSpritesImpl.h"

#include "dataset/Group.h"
#include "dataset/ISprite.h"

#include <sstream>
#include <wx/menu.h>

namespace d2d
{

BEGIN_EVENT_TABLE(GroupTreeCtrl, wxTreeCtrl)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_GROUP_TREE_CTRL, GroupTreeCtrl::OnItemRClick)
	EVT_TREE_ITEM_MENU(ID_GROUP_TREE_CTRL, GroupTreeCtrl::OnItemMenu)
	EVT_TREE_ITEM_ACTIVATED(ID_GROUP_TREE_CTRL, GroupTreeCtrl::OnItemActivated)
END_EVENT_TABLE()

GroupTreeCtrl::GroupTreeCtrl(wxWindow* parent, MultiSpritesImpl* sprite_impl)
	: wxTreeCtrl(parent, ID_GROUP_TREE_CTRL, wxDefaultPosition, wxDefaultSize, wxTR_EDIT_LABELS)
	, m_sprite_impl(sprite_impl)
{
	m_root = AddRoot("Root");
}

void GroupTreeCtrl::AddNode()
{
	static int s_num = 0;

	std::ostringstream ss;
	ss << "node" << s_num++;
	std::string text = ss.str();

	ItemData* data = new ItemData;
	data->group = new Group(text);

	wxTreeItemId id = GetFocusedItem();
	if (id.IsOk()) {
		AddNode(id, text, data);
	} else {
		AddNode(m_root, text, data);
	}
}

void GroupTreeCtrl::DelNode()
{
	wxTreeItemId id = GetFocusedItem();
	if (id.IsOk()) {
		Delete(id);
	}
}

void GroupTreeCtrl::Clear()
{
	if (!IsEmpty()) {
		CollapseAndReset(m_root);
	}
}

void GroupTreeCtrl::AddNode(wxTreeItemId parent, const std::string& name, ItemData* data)
{
	wxTreeItemId id = AppendItem(parent, name, -1, -1, data);
}

void GroupTreeCtrl::OnItemRClick(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();

	ItemData* item = (ItemData*)GetItemData(itemId);

	event.Skip();
}

void GroupTreeCtrl::OnItemMenu(wxTreeEvent& event)
{
	wxTreeItemId id = event.GetItem();
	if (!id.IsOk()) {
		return;
	}	

	wxPoint clientpt = event.GetPoint();
	wxPoint screenpt = ClientToScreen(clientpt);
	ShowMenu(id, clientpt);
	event.Skip();
}

void GroupTreeCtrl::OnItemActivated(wxTreeEvent& event)
{
	wxTreeItemId id = event.GetItem();
	ItemData* data = (ItemData*)GetItemData(id);

	SpriteSelection* selection = m_sprite_impl->getSpriteSelection();
	selection->Clear();
	if (data->group) 
	{
		std::vector<ISprite*> sprites;
		data->group->TraverseSprite(FetchAllVisitor<ISprite>(sprites));
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			selection->Add(sprites[i]);
		}
	} 
	else if (data->sprite) 
	{
		selection->Add(data->sprite);
	}
}

void GroupTreeCtrl::OnMenuAddSprites(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}	

	Group* group = ((ItemData*)GetItemData(m_on_menu_id))->group;
	if (!group) {
		return;
	}

	SpriteSelection* selection = m_sprite_impl->getSpriteSelection();
	std::vector<ISprite*> sprites;
	selection->Traverse(FetchAllVisitor<ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ISprite* spr = sprites[i];
		bool ok = group->Insert(spr);
		if (ok) {
			ItemData* data = new ItemData;
			data->sprite = spr;
			AddNode(m_on_menu_id, spr->name, data);
		}
	}
}

void GroupTreeCtrl::OnMenuClear(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}	

	Group* group = ((ItemData*)GetItemData(m_on_menu_id))->group;
	if (!group) {
		return;
	}

	group->Clear();
	CollapseAndReset(m_on_menu_id);
}

void GroupTreeCtrl::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
	m_on_menu_id = id;

	wxMenu menu;

	Bind(wxEVT_COMMAND_MENU_SELECTED, &GroupTreeCtrl::OnMenuAddSprites, this, ID_ADD_SPRITES);
	menu.Append(ID_ADD_SPRITES, wxT("&Add Sprites"));

	PopupMenu(&menu, pt);
}

}
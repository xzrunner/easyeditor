#include "GroupTreeCtrl.h"
#include "MultiSpritesImpl.h"

#include "dataset/Group.h"
#include "dataset/ISprite.h"

#include <sstream>
#include <queue>
#include <wx/menu.h>

namespace d2d
{

BEGIN_EVENT_TABLE(GroupTreeCtrl, wxTreeCtrl)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_GROUP_TREE_CTRL, GroupTreeCtrl::OnItemRClick)
	EVT_TREE_ITEM_MENU(ID_GROUP_TREE_CTRL, GroupTreeCtrl::OnItemMenu)
	EVT_TREE_ITEM_ACTIVATED(ID_GROUP_TREE_CTRL, GroupTreeCtrl::OnItemActivated)
END_EVENT_TABLE()

GroupTreeCtrl::GroupTreeCtrl(wxWindow* parent, MultiSpritesImpl* sprite_impl)
	: wxTreeCtrl(parent, ID_GROUP_TREE_CTRL, wxDefaultPosition, wxDefaultSize, 
	wxTR_EDIT_LABELS | wxTR_HIDE_ROOT)
	, m_sprite_impl(sprite_impl)
{
	m_root = AddRoot("Root");
}

void GroupTreeCtrl::Traverse(IVisitor& visitor)
{
	std::queue<wxTreeItemId> buf;
	buf.push(m_root);
	while (!buf.empty()) {
		wxTreeItemId item = buf.front(); buf.pop();

		wxTreeItemIdValue cookie;
		wxTreeItemId id = GetFirstChild(item, cookie);
		if (id.IsOk()) {
			if (item != m_root) {
				visitor.VisitNonleaf(item);
			}

			while (id.IsOk()) {
				buf.push(id);
				id = GetNextSibling(id);
			}
		} else {
			visitor.VisitLeaf(item);
		}
	}
}

void GroupTreeCtrl::AddNode()
{
	static int s_num = 0;

	std::ostringstream ss;
	ss << "Group" << s_num++;
	std::string text = ss.str();

	ItemData* data = new ItemData(new Group(text));

	wxTreeItemId id = GetFocusedItem();
	if (id.IsOk()) {
		data->SetId(id);
		AddNode(id, text, data);
	} else {
		data->SetId(m_root);
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

void GroupTreeCtrl::Remove(ISprite* sprite)
{
	Traverse(RemoveVisitor(this, sprite));
}

void GroupTreeCtrl::AddNode(wxTreeItemId parent, const std::string& name, ItemData* data)
{
	wxTreeItemId id = AppendItem(parent, name, -1, -1, data);
	ExpandAll();
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
	if (data->m_group) 
	{
		std::vector<ISprite*> sprites;
		data->m_group->TraverseSprite(FetchAllVisitor<ISprite>(sprites));
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			selection->Add(sprites[i]);
		}
	} 
	else if (data->m_sprite) 
	{
		selection->Add(data->m_sprite);
	}
}

void GroupTreeCtrl::OnMenuAddSprites(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}	

	ItemData* data = (ItemData*)GetItemData(m_on_menu_id);
	if (!data) {
		return;
	}

	Group* group = data->m_group;
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
			ItemData* data = new ItemData(spr);
			AddNode(m_on_menu_id, spr->name, data);
		}
	}
}

void GroupTreeCtrl::OnMenuClear(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}	

	Group* group = ((ItemData*)GetItemData(m_on_menu_id))->m_group;
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

	menu.Append(ID_MENU_ADD, wxT("&Add Sprites"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &GroupTreeCtrl::OnMenuAddSprites, this, ID_MENU_ADD);

	menu.Append(ID_MENU_CLEAR, wxT("&Clear Sprites"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &GroupTreeCtrl::OnMenuClear, this, ID_MENU_CLEAR);

	PopupMenu(&menu, pt);
}

//////////////////////////////////////////////////////////////////////////
// class GroupTreeCtrl::ItemData
//////////////////////////////////////////////////////////////////////////

GroupTreeCtrl::ItemData::
ItemData(Group* group)
	: m_group(group)
	, m_sprite(NULL)
{
	if (m_group) {
		m_group->Retain();
	}
}

GroupTreeCtrl::ItemData::
ItemData(ISprite* sprite)
	: m_sprite(sprite)
	, m_group(NULL)
{
	if (m_sprite) {
		m_sprite->Retain();
	}
}

GroupTreeCtrl::ItemData::
~ItemData()
{
	if (m_group) {
		m_group->Release();
	}
	if (m_sprite) {
		m_sprite->Release();
	}
}

//////////////////////////////////////////////////////////////////////////
// class GroupTreeCtrl::RemoveVisitor
//////////////////////////////////////////////////////////////////////////

GroupTreeCtrl::RemoveVisitor::
RemoveVisitor(wxTreeCtrl* treectrl, d2d::ISprite* spr)
	: m_treectrl(treectrl)
	, m_spr(spr)
{
	if (m_spr) {
		m_spr->Retain();
	}
}

GroupTreeCtrl::RemoveVisitor::
~RemoveVisitor()
{
	if (m_spr) {
		m_spr->Release();
	}
}

void GroupTreeCtrl::RemoveVisitor::
VisitNonleaf(wxTreeItemId id)
{
	assert(id.IsOk());

	ItemData* data = (ItemData*)m_treectrl->GetItemData(id);
	if (!data) {
		return;
	}

	if (data->m_group) {
		data->m_group->Remove(m_spr);
	}
}

void GroupTreeCtrl::RemoveVisitor::
VisitLeaf(wxTreeItemId id)
{
	assert(id.IsOk());
	ItemData* data = (ItemData*)m_treectrl->GetItemData(id);
	if (!data) {
		return;
	}

	if (data->m_sprite && data->m_sprite == m_spr) {
		delete data;
		m_treectrl->Delete(id);
	}
}

}
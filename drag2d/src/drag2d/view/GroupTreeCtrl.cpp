#include "GroupTreeCtrl.h"
#include "GroupTreePanel.h"
#include "MultiSpritesImpl.h"
#include "GroupTreeImpl.h"
#include "ViewPanelMgr.h"

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
	EVT_TREE_BEGIN_DRAG(ID_GROUP_TREE_CTRL, GroupTreeCtrl::OnBeginDrag)
	EVT_TREE_END_DRAG(ID_GROUP_TREE_CTRL, GroupTreeCtrl::OnEndDrag)
	EVT_KEY_DOWN(GroupTreeCtrl::OnKeyDown)
	EVT_TREE_SEL_CHANGED(ID_GROUP_TREE_CTRL, GroupTreeCtrl::OnSelChanged)
END_EVENT_TABLE()

GroupTreeCtrl::GroupTreeCtrl(GroupTreePanel* parent, MultiSpritesImpl* sprite_impl,
							 ViewPanelMgr* view_panel_mgr)
	: wxTreeCtrl(parent, ID_GROUP_TREE_CTRL, wxDefaultPosition, wxDefaultSize, 
	wxTR_EDIT_LABELS/* | wxTR_MULTIPLE*/ | wxTR_NO_LINES | wxTR_DEFAULT_STYLE)
	, m_parent_panel(parent)
	, m_sprite_impl(sprite_impl)
	, m_view_panel_mgr(view_panel_mgr)
	, m_add_del_open(true)
{
	SetBackgroundColour(wxColour(229, 229, 229));

	InitRoot();
}

void GroupTreeCtrl::Traverse(IGroupTreeVisitor& visitor) const
{
	Traverse(m_root, visitor);
}

void GroupTreeCtrl::Traverse(wxTreeItemId id, IGroupTreeVisitor& visitor) const
{
	std::queue<wxTreeItemId> buf;
	buf.push(id);
	while (!buf.empty()) {
		wxTreeItemId item = buf.front(); buf.pop();

		wxTreeItemIdValue cookie;
		wxTreeItemId id = GetFirstChild(item, cookie);
		if (id.IsOk()) {
			if (item != id) {
				bool finish = visitor.VisitNonleaf(item);
				if (finish) {
					break;
				}
			}

			while (id.IsOk()) {
				buf.push(id);
				id = GetNextSibling(id);
			}
		} else {
			bool finish = visitor.VisitLeaf(item);
			if (finish) {
				break;
			}
		}
	}
}

wxTreeItemId GroupTreeCtrl::AddNode()
{
	static int s_num = 0;

	std::ostringstream ss;
	ss << "GROUP_" << s_num++;
	std::string text = ss.str();

	return AddNode(text);
}

wxTreeItemId GroupTreeCtrl::AddNode(const std::string& name)
{
	GroupTreeItem* data = new GroupTreeGroupItem(new Group(name));

	wxTreeItemId ret;
	wxTreeItemId id = GetFocusedItem();
	if (id.IsOk() && ((GroupTreeItem*)GetItemData(id))->IsGroup()) {
		data->SetId(id);
		ret = AddNode(id, name, data);
	} else {
		data->SetId(m_root);
		ret = AddNode(m_root, name, data);
	}
	SetItemBold(ret, true);
	SelectItem(ret);
	return ret;
}

wxTreeItemId GroupTreeCtrl::AddNode(const std::string& name, wxTreeItemId parent,
									bool visible, bool editable)
{
	GroupTreeItem* data = new GroupTreeGroupItem(new Group(name, visible, editable));
	wxTreeItemId ret = AddNode(parent, name, data);
	SetItemBold(ret, true);
	SelectItem(ret);
	return ret;
}

void GroupTreeCtrl::DelNode()
{
	wxTreeItemId id = GetFocusedItem();
	if (id.IsOk()) {
		Delete(id);
	}
}

wxTreeItemId GroupTreeCtrl::AddSprite(wxTreeItemId parent, d2d::ISprite* spr)
{
	if (!parent.IsOk()) {
		return m_root;
	}	

	GroupTreeItem* data = (GroupTreeItem*)GetItemData(parent);
	if (!data || !data->IsGroup()) {
		return m_root;
	}

	Group* group = static_cast<GroupTreeGroupItem*>(data)->GetGroup();
	bool ok = group->Insert(spr);
	if (ok) {
		GroupTreeItem* data = new GroupTreeSpriteItem(spr);
		return AddNode(parent, spr->name, data);
	} else {
		return m_root;
	}
}

wxTreeItemId GroupTreeCtrl::AddSprite(d2d::ISprite* spr)
{
	if (!m_add_del_open) {
		return m_root;
	}

	wxTreeItemId ret;
	if (m_selected_item.IsOk()) {
		ret = AddSprite(m_selected_item, spr);
	}
	if (!ret.IsOk() || ret == m_root) {
		assert(m_root.IsOk());
		ret = AddSprite(m_root, spr);
	}
	return ret;
}

void GroupTreeCtrl::Clear()
{
	if (IsEmpty()) {
		return;
	}

	GroupTreeItem* data = (GroupTreeItem*)GetItemData(m_root);
	assert(data && data->IsGroup());
	Group* group = static_cast<GroupTreeGroupItem*>(data)->GetGroup();
	group->Clear();
	this->ClearFocusedItem();

	CollapseAndReset(m_root);
}

void GroupTreeCtrl::Remove(ISprite* sprite)
{
	if (m_add_del_open) {
		Traverse(GroupTreeImpl::RemoveVisitor(this, sprite));
	}
}

void GroupTreeCtrl::ReorderItem(wxTreeItemId id, bool up)
{
	if (!id.IsOk()) {
		return;
	}

	wxTreeItemId insert_prev;
	if (up) {
		wxTreeItemId prev = GetPrevSibling(id);
		if (!prev.IsOk()) {
			return;
		}
		insert_prev = GetPrevSibling(prev);
	} else {
		wxTreeItemId next = GetNextSibling(id);
		if (!next.IsOk()) {
			return;
		}
		insert_prev = next;
	}

	// old info
	GroupTreeItem* data = (GroupTreeItem*)GetItemData(id);
	std::string name = GetItemText(id);
	// insert
	wxTreeItemId parent = GetItemParent(id);
	wxTreeItemId new_item;
	if (insert_prev.IsOk()) {
		new_item = InsertItem(parent, insert_prev, name, -1, -1, data->Clone());
	} else {
		new_item = InsertItem(parent, 0, name, -1, -1, data->Clone());
	}
	ExpandAll();
	// font style
	if (data->IsGroup()) {
		SetItemBold(new_item, true);
	}
	// copy older's children
	Traverse(id, GroupTreeImpl::CopyPasteVisitor(this, id, new_item));
	// remove
	Delete(id);
	// sort
//	ReorderSprites();
	// set selection
	SelectItem(new_item);
}

wxTreeItemId GroupTreeCtrl::AddNode(wxTreeItemId parent, const std::string& name, GroupTreeItem* data)
{
	wxTreeItemId id = InsertItem(parent, 0, name, -1, -1, data);
	ExpandAll();
	return id;
}

void GroupTreeCtrl::InitRoot()
{
	GroupTreeItem* data = new GroupTreeGroupItem(new Group("ROOT"));
	m_root = AddRoot("ROOT");
	SetItemBold(m_root, true);
	SetItemData(m_root, data);
}

void GroupTreeCtrl::OnItemRClick(wxTreeEvent& event)
{
	wxTreeItemId itemId = event.GetItem();

	GroupTreeItem* item = (GroupTreeItem*)GetItemData(itemId);

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

	GroupTreeImpl::GetFirstSpriteVisitor visitor(this);
	Traverse(id, visitor);
	ISprite* spr = visitor.GetFirstSprite();

	bool add = wxGetKeyState(WXK_CONTROL);
	m_view_panel_mgr->SelectSprite(spr, !add, m_parent_panel);

	SpriteSelection* selection = m_sprite_impl->getSpriteSelection();
	selection->Clear();
	Traverse(id, GroupTreeImpl::SelectVisitor(this, selection));
}

void GroupTreeCtrl::OnBeginDrag(wxTreeEvent& event)
{
	m_dragged_item = event.GetItem();
	event.Allow();
}

void GroupTreeCtrl::OnEndDrag(wxTreeEvent& event)
{
	wxTreeItemId item_src = m_dragged_item,
		item_dst = event.GetItem();
	wxLogDebug("Drag from %s to %s", GetItemText(item_src), GetItemText(item_dst));

	if (!item_src.IsOk() || !item_dst.IsOk()) {
		return;
	}
	GroupTreeItem* data_dst = (GroupTreeItem*)GetItemData(item_dst);
	if (!data_dst || !data_dst->IsGroup()) {
		return;
	}

	// old info
	GroupTreeItem* data = (GroupTreeItem*)GetItemData(item_src);
	std::string name = GetItemText(item_src);
	// insert
	wxTreeItemId new_item = InsertItem(item_dst, 0, name, -1, -1, data->Clone());
	ExpandAll();
	// copy older's children
	Traverse(item_src, GroupTreeImpl::CopyPasteVisitor(this, item_src, new_item));
	// remove
	Delete(item_src);
	// sort
//	ReorderSprites();
	// set selection
	SelectItem(new_item);
	// font style
	if (data->IsGroup()) {
		SetItemBold(new_item, true);
	}
}

void GroupTreeCtrl::OnKeyDown(wxKeyEvent& event)
{
	switch (event.GetKeyCode())
	{
	case WXK_UP:
		SelectUp();
		break;
	case WXK_DOWN:
		SelectDown();
		break;
	case WXK_LEFT:
		SelectLeft();
		break;
	case WXK_RIGHT:
		SelectRight();
		break;
	case WXK_PAGEUP:
		ReorderItem(m_selected_item, true);
		break;
	case WXK_PAGEDOWN:
		ReorderItem(m_selected_item, false);
		break;
	}
}

void GroupTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	m_selected_item = event.GetItem();

	wxLogDebug("OnSelChanged %s", GetItemText(m_selected_item));

	GroupTreeItem* data = (GroupTreeItem*)GetItemData(m_selected_item);
	if (data && !data->IsGroup()) {
		ISprite* spr = static_cast<GroupTreeSpriteItem*>(data)->GetSprite();
		bool add = wxGetKeyState(WXK_CONTROL);
		m_view_panel_mgr->SelectSprite(spr, !add, m_parent_panel);
	}
}

void GroupTreeCtrl::OnMenuAddSprites(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}

	GroupTreeItem* data = (GroupTreeItem*)GetItemData(m_on_menu_id);
	if (!data || !data->IsGroup()) {
		return;
	}

	SpriteSelection* selection = m_sprite_impl->getSpriteSelection();
	std::vector<ISprite*> sprites;
	selection->Traverse(FetchAllVisitor<ISprite>(sprites));
	Group* group = static_cast<GroupTreeGroupItem*>(data)->GetGroup();
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ISprite* spr = sprites[i];
		bool ok = group->Insert(spr);
		if (ok) {
			GroupTreeItem* data = new GroupTreeSpriteItem(spr);
			AddNode(m_on_menu_id, spr->name, data);
		}
	}
}

void GroupTreeCtrl::OnMenuClear(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}	

	GroupTreeItem* data = (GroupTreeItem*)GetItemData(m_on_menu_id);
	if (!data || !data->IsGroup()) {
		return;
	}

	Group* group = static_cast<GroupTreeGroupItem*>(data)->GetGroup();
	group->Clear();
	CollapseAndReset(m_on_menu_id);
}

void GroupTreeCtrl::OnMenuVisible(wxCommandEvent& event)
{
	Traverse(m_on_menu_id, GroupTreeImpl::VisibleVisitor(this));
}

void GroupTreeCtrl::OnMenuEditable(wxCommandEvent& event)
{
	Traverse(m_on_menu_id, GroupTreeImpl::EditableVisitor(this));
}

void GroupTreeCtrl::SelectUp()
{
	if (!m_selected_item.IsOk()) {
		return;
	}

	wxTreeItemId prev = GetPrevSibling(m_selected_item);
	if (prev.IsOk()) {
		SelectItem(prev); 
	}
}

void GroupTreeCtrl::SelectDown()
{
	if (!m_selected_item.IsOk()) {
		return;
	}

	wxTreeItemId next = GetNextSibling(m_selected_item);
	if (next.IsOk()) {
		SelectItem(next); 
	}
}

void GroupTreeCtrl::SelectLeft()
{
	if (!m_selected_item.IsOk()) {
		return;
	}

	if (ItemHasChildren(m_selected_item) && IsExpanded(m_selected_item)) {
		Collapse(m_selected_item);
	} else {
	 	wxTreeItemId parent = GetItemParent(m_selected_item);
	 	if (parent.IsOk() && parent != m_root) {
	 		SelectItem(parent);
	 	}
	}
}

void GroupTreeCtrl::SelectRight()
{
	if (!m_selected_item.IsOk()) {
		return;
	}

	if (ItemHasChildren(m_selected_item)) {
		if (IsExpanded(m_selected_item)) {
			wxTreeItemIdValue cookie;
			wxTreeItemId next = GetFirstChild(m_selected_item, cookie);
			if (next.IsOk()) {
				SelectItem(next);
			}
		} else {
			Expand(m_selected_item);
		}
	}
}

void GroupTreeCtrl::ReorderSprites()
{
	std::vector<ISprite*> sprites;
	Traverse(m_root, GroupTreeImpl::GetSpritesVisitor(this, sprites));
	for (int i = sprites.size() - 1; i >= 0; --i) {
		ISprite* spr = sprites[i];
		m_add_del_open = false;
		m_sprite_impl->removeSprite(spr);
		m_sprite_impl->insertSprite(spr);
		m_add_del_open = true;
	}
}

void GroupTreeCtrl::ShowMenu(wxTreeItemId id, const wxPoint& pt)
{
	m_on_menu_id = id;

	wxMenu menu;

	menu.Append(ID_MENU_ADD, wxT("&Add Sprites"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &GroupTreeCtrl::OnMenuAddSprites, this, ID_MENU_ADD);

	menu.Append(ID_MENU_CLEAR, wxT("&Clear Sprites"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &GroupTreeCtrl::OnMenuClear, this, ID_MENU_CLEAR);

	menu.AppendSeparator();

	menu.AppendCheckItem(ID_MENU_VISIBLE, wxT("&Visible"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &GroupTreeCtrl::OnMenuVisible, this, ID_MENU_VISIBLE);

	menu.AppendCheckItem(ID_MENU_EDITABLE, wxT("&Editable"));
	Bind(wxEVT_COMMAND_MENU_SELECTED, &GroupTreeCtrl::OnMenuEditable, this, ID_MENU_EDITABLE);

	PopupMenu(&menu, pt);
}

}
#include "GroupTreeCtrl.h"
#include "MultiSpritesImpl.h"
#include "GroupTreeImpl.h"
#include "GroupTreeIO.h"

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

GroupTreeCtrl::GroupTreeCtrl(wxWindow* parent, MultiSpritesImpl* sprite_impl)
	: wxTreeCtrl(parent, ID_GROUP_TREE_CTRL, wxDefaultPosition, wxDefaultSize, 
	wxTR_EDIT_LABELS | wxTR_HIDE_ROOT | wxTR_NO_LINES | wxTR_DEFAULT_STYLE)
	, m_sprite_impl(sprite_impl)
	, m_remove_open(true)
{
	m_root = AddRoot("Root");
}

void GroupTreeCtrl::StoreToFile(Json::Value& value) const
{
	Traverse(GroupTreeImpl::StoreVisitor(this, value));
}

void GroupTreeCtrl::LoadFromFile(const Json::Value& value)
{
	GroupTreeIO io(this, m_sprite_impl);
	io.Load(value);
}

void GroupTreeCtrl::Traverse(IGroupTreeVisitor& visitor) const
{
	Traverse(m_root, visitor);
}

wxTreeItemId GroupTreeCtrl::AddNode()
{
	static int s_num = 0;

	std::ostringstream ss;
	ss << "Group" << s_num++;
	std::string text = ss.str();

	return AddNode(text);
}

wxTreeItemId GroupTreeCtrl::AddNode(const std::string& name)
{
	GroupTreeItem* data = new GroupTreeItem(new Group(name));

	wxTreeItemId id = GetFocusedItem();
	if (id.IsOk()) {
		data->SetId(id);
		return AddNode(id, name, data);
	} else {
		data->SetId(m_root);
		return AddNode(m_root, name, data);
	}
}

wxTreeItemId GroupTreeCtrl::AddNode(const std::string& name, wxTreeItemId parent)
{
	GroupTreeItem* data = new GroupTreeItem(new Group(name));
	return AddNode(parent, name, data);
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
	if (!data) {
		return m_root;
	}

	Group* group = data->m_group;
	if (!group) {
		return m_root;
	}

	bool ok = group->Insert(spr);
	if (ok) {
		GroupTreeItem* data = new GroupTreeItem(spr);
		return AddNode(parent, spr->name, data);
	} else {
		return m_root;
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
	if (m_remove_open) {
		Traverse(GroupTreeImpl::RemoveVisitor(this, sprite));
	}
}

void GroupTreeCtrl::Visible(wxTreeItemId id, bool visible)
{
	Traverse(id, GroupTreeImpl::SetVisibleVisitor(this, visible));
}

void GroupTreeCtrl::Editable(wxTreeItemId id, bool editable)
{
	Traverse(id, GroupTreeImpl::SetEditableVisitor(this, editable));
}

wxTreeItemId GroupTreeCtrl::AddNode(wxTreeItemId parent, const std::string& name, GroupTreeItem* data)
{
	wxTreeItemId id = InsertItem(parent, 0, name, -1, -1, data);
	ExpandAll();
	return id;
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
	if (!item_dst.IsOk()) {
		return;
	}

	wxLogDebug("Drag from %s to %s", GetItemText(item_src), GetItemText(item_dst));

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
	ReorderSprites();
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
		ReorderSelected(true);
		break;
	case WXK_PAGEDOWN:
		ReorderSelected(false);
		break;
	}
}

void GroupTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	m_selected_item = event.GetItem();
//	wxLogDebug("OnSelChanged %s", GetItemText(m_selected_item));
}

void GroupTreeCtrl::OnMenuAddSprites(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}

	GroupTreeItem* data = (GroupTreeItem*)GetItemData(m_on_menu_id);
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
			GroupTreeItem* data = new GroupTreeItem(spr);
			AddNode(m_on_menu_id, spr->name, data);
		}
	}
}

void GroupTreeCtrl::OnMenuClear(wxCommandEvent& event)
{
	if (!m_on_menu_id.IsOk()) {
		return;
	}	

	Group* group = ((GroupTreeItem*)GetItemData(m_on_menu_id))->m_group;
	if (!group) {
		return;
	}

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

void GroupTreeCtrl::ReorderSelected(bool up)
{
	if (!m_selected_item.IsOk()) {
		return;
	}

	wxTreeItemId insert_prev;
	if (up) {
		wxTreeItemId prev = GetPrevSibling(m_selected_item);
		if (!prev.IsOk()) {
			return;
		}
		insert_prev = GetPrevSibling(prev);
	} else {
		wxTreeItemId next = GetNextSibling(m_selected_item);
		if (!next.IsOk()) {
			return;
		}
		insert_prev = next;
	}

 	// old info
 	GroupTreeItem* data = (GroupTreeItem*)GetItemData(m_selected_item);
 	std::string name = GetItemText(m_selected_item);

	// insert
	wxTreeItemId parent = GetItemParent(m_selected_item);
	wxTreeItemId new_item;
	if (insert_prev.IsOk()) {
		new_item = InsertItem(parent, insert_prev, name, -1, -1, data->Clone());
	} else {
		new_item = InsertItem(parent, 0, name, -1, -1, data->Clone());
	}
	ExpandAll();
 	// copy older's children
 	Traverse(m_selected_item, GroupTreeImpl::CopyPasteVisitor(this, m_selected_item, new_item));
 	// remove
 	Delete(m_selected_item);
 	// sort
 	ReorderSprites();
	// set selection
	SelectItem(new_item);
}

void GroupTreeCtrl::ReorderSprites()
{
	std::vector<ISprite*> sprites;
	Traverse(m_root, GroupTreeImpl::GetSpritesVisitor(this, sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		ISprite* spr = sprites[i];
		m_remove_open = false;
		m_sprite_impl->removeSprite(spr);
		m_remove_open = true;
		m_sprite_impl->insertSprite(spr);
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

}
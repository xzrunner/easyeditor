#include "GroupTreeCtrl.h"
#include "GroupTreePanel.h"
#include "MultiSpritesImpl.h"
#include "GroupTreeImpl.h"

#include "dataset/Group.h"
#include "dataset/ISprite.h"
#include "view/SpriteSelection.h"
#include "view/KeysState.h"

#include "message/subject_id.h"
#include "message/SpriteNameChangeSJ.h"
#include "message/SelectSpriteSJ.h"
#include "message/SelectSpriteSetSJ.h"
#include "message/ReorderSpriteSJ.h"
#include "message/InsertSpriteSJ.h"
#include "message/RemoveSpriteSJ.h"
#include "message/ClearSpriteSJ.h"

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
	EVT_TREE_END_LABEL_EDIT(ID_GROUP_TREE_CTRL, GroupTreeCtrl::OnLabelEdited)
END_EVENT_TABLE()

GroupTreeCtrl::GroupTreeCtrl(GroupTreePanel* parent, MultiSpritesImpl* sprite_impl,
							 const KeysState& key_state)
	: wxTreeCtrl(parent, ID_GROUP_TREE_CTRL, wxDefaultPosition, wxDefaultSize, 
	wxTR_EDIT_LABELS/* | wxTR_MULTIPLE*/ | wxTR_NO_LINES | wxTR_DEFAULT_STYLE)
	, m_parent_panel(parent)
	, m_sprite_impl(sprite_impl)
	, m_add_del_open(true)
	, m_key_state(key_state)
	, m_expand_enable(true)
	, m_select_enable(true)
{
	SetBackgroundColour(wxColour(229, 229, 229));

	InitRoot();

	m_subjects.push_back(SpriteNameChangeSJ::Instance());
	m_subjects.push_back(SelectSpriteSJ::Instance());
	m_subjects.push_back(SelectSpriteSetSJ::Instance());
	m_subjects.push_back(ReorderSpriteSJ::Instance());
	m_subjects.push_back(InsertSpriteSJ::Instance());
	m_subjects.push_back(RemoveSpriteSJ::Instance());
	m_subjects.push_back(ClearSpriteSJ::Instance());
	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->Register(this);
	}
}

GroupTreeCtrl::~GroupTreeCtrl()
{
	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->UnRegister(this);
	}
}

void GroupTreeCtrl::Notify(int sj_id, void* ud)
{
	switch (sj_id) 
	{
	case MSG_SPRITE_NAME_CHANGE:
		ChangeName((ISprite*)ud);
		break;
	case MSG_SELECT_SPRITE:
		{
			SelectSpriteSJ::Params* p = (SelectSpriteSJ::Params*)ud;
			Select(p->spr, p->clear);	
		}
		break;
	case MSG_SELECT_SPRITE_SET:
		{
			//SpriteSelection* selection = (SpriteSelection*)ud;
			//OnMultiSpriteSelected(selection);
		}
		break;
	case MSG_REORDER_SPRITE:
		{
			ReorderSpriteSJ::Params* p = (ReorderSpriteSJ::Params*)ud;
			Reorder(p->spr, p->up);
		}
		break;
	case MSG_INSERT_SPRITE:
		{
			InsertSpriteSJ::Params* p = (InsertSpriteSJ::Params*)ud;
			AddSprite(p->spr);
		}
		break;
	case MSG_REMOVE_SPRITE:
		Remove((ISprite*)ud);
		break;
	case MSG_CLEAR_SPRITE:
		Clear();
		break;
	}
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
	if (!id.IsOk()) {
		return;
	}

	std::vector<ISprite*> sprites;
	Traverse(id, GroupTreeImpl::GetSpritesVisitor(this, sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		RemoveSpriteSJ::Instance()->Remove(sprites[i], this);
	}

	Delete(id);
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

bool GroupTreeCtrl::ReorderItem(wxTreeItemId id, bool up)
{
	if (!id.IsOk()) {
		return false;
	}

	wxTreeItemId insert_prev;
	if (up) {
		wxTreeItemId prev = GetPrevSibling(id);
		if (!prev.IsOk()) {
			return false;
		}
		insert_prev = GetPrevSibling(prev);
	} else {
		wxTreeItemId next = GetNextSibling(id);
		if (!next.IsOk()) {
			return false;
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

	return true;
}

wxTreeItemId GroupTreeCtrl::AddNode(wxTreeItemId parent, const std::string& name, 
									GroupTreeItem* data)
{
	wxTreeItemId id = InsertItem(parent, 0, name, -1, -1, data);
	if (m_expand_enable) {
		ExpandAll();
	}
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

	bool add = m_key_state.GetKeyState(WXK_CONTROL);
	SelectSpriteSJ::Instance()->Select(spr, !add, this);

	SpriteSelection* selection = m_sprite_impl->GetSpriteSelection();
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
	bool is_group = data->IsGroup();
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
	if (is_group) {
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
	case WXK_DELETE:
		DelNode();
		break;
	}
}

void GroupTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	if (!m_select_enable) {
		m_select_enable = true;
		return;
	}

	m_selected_item = event.GetItem();

	GroupTreeItem* data = (GroupTreeItem*)GetItemData(m_selected_item);
	if (data && !data->IsGroup()) {
		ISprite* spr = static_cast<GroupTreeSpriteItem*>(data)->GetSprite();
		bool add = m_key_state.GetKeyState(WXK_CONTROL);
		SelectSpriteSJ::Instance()->Select(spr, !add, this);
	}
}

void GroupTreeCtrl::OnLabelEdited(wxTreeEvent& event)
{
	m_selected_item = event.GetItem();
	GroupTreeItem* data = (GroupTreeItem*)GetItemData(m_selected_item);
	if (data && !data->IsGroup()) {
		ISprite* spr = static_cast<GroupTreeSpriteItem*>(data)->GetSprite();
		spr->name = event.GetLabel();
		SpriteNameChangeSJ::Instance()->OnSpriteNameChanged(spr, this);
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

	SpriteSelection* selection = m_sprite_impl->GetSpriteSelection();
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
		RemoveSpriteSJ::Instance()->Remove(spr);
		InsertSpriteSJ::Instance()->Insert(spr);
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

void GroupTreeCtrl::ChangeName(ISprite* spr)
{
	GroupTreeImpl::QuerySpriteVisitor visitor(this, spr);
	Traverse(visitor);
	wxTreeItemId id = visitor.GetItemID();

	SetItemText(id, spr->name);
}

void GroupTreeCtrl::Select(d2d::ISprite* spr, bool clear)
{
	GroupTreeImpl::QuerySpriteVisitor visitor(this, spr);
	Traverse(visitor);
	wxTreeItemId id = visitor.GetItemID();
	if (id.IsOk()) {
		m_select_enable = false;
		SelectItem(id);
	}
}

void GroupTreeCtrl::SelectSet(SpriteSelection* selection)
{
	std::vector<ISprite*> sprites;
	selection->Traverse(FetchAllVisitor<ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		Select(sprites[i], false);
	}
}

void GroupTreeCtrl::Reorder(ISprite* spr, bool up)
{
	GroupTreeImpl::QuerySpriteVisitor visitor(this, spr);
	Traverse(visitor);
	wxTreeItemId id = visitor.GetItemID();
	if (id.IsOk()) {
		ReorderItem(id, up);
	}
}

bool GroupTreeCtrl::Remove(ISprite* sprite)
{
	if (m_add_del_open) {
		GroupTreeImpl::RemoveVisitor visitor(this, sprite);
		Traverse(visitor);
		return visitor.IsFinish();
	} else {
		return false;
	}
}

}
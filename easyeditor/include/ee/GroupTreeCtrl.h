#ifndef _EASYEDITOR_GROUP_TREE_CTRL_H_
#define _EASYEDITOR_GROUP_TREE_CTRL_H_

#include "GroupTreeVisitor.h"
#include "GroupTreeItem.h"
#include "Observer.h"

#include <wx/treectrl.h>
#include <json/json.h>

namespace ee
{

class GroupTreePanel;
class Sprite;
class Group;
class MultiSpritesImpl;
class KeysState;
class SpriteSelection;

class GroupTreeCtrl : public wxTreeCtrl, public Observer
{
public:
	GroupTreeCtrl(GroupTreePanel* parent, MultiSpritesImpl* sprite_impl,
		const KeysState& key_state);

	void Clear();

	void Traverse(IGroupTreeVisitor& visitor) const;
	void Traverse(wxTreeItemId id, IGroupTreeVisitor& visitor) const;

	wxTreeItemId AddNode();
	wxTreeItemId AddNode(const std::string& name);
	wxTreeItemId AddNode(const std::string& name, wxTreeItemId parent,
		bool visible = true, bool editable = true);
	void DelNode();

	wxTreeItemId AddSprite(wxTreeItemId parent, const SprPtr& spr);
	wxTreeItemId AddSprite(const SprPtr& spr);

	wxTreeItemId GetRootID() const { return m_root; }

	bool ReorderItem(wxTreeItemId id, bool up);

	void EnableExpand(bool enable) { m_expand_enable = enable; }

protected:
	virtual void OnNotify(int sj_id, void* ud) override;

private:
	wxTreeItemId AddNode(wxTreeItemId parent, const std::string& name, 
		GroupTreeItem* data);

private:
	void InitRoot();

	void OnItemRClick(wxTreeEvent& event);
	void OnItemMenu(wxTreeEvent& event);
	void OnItemActivated(wxTreeEvent& event);
	void OnBeginDrag(wxTreeEvent& event);
	void OnEndDrag(wxTreeEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnSelChanged(wxTreeEvent& event);
	void OnLabelEdited(wxTreeEvent& event);

	// menu
	void OnMenuAddSprites(wxCommandEvent& event);
	void OnMenuClear(wxCommandEvent& event);
	void OnMenuVisible(wxCommandEvent& event);
	void OnMenuEditable(wxCommandEvent& event);

	// select
	void SelectUp();
	void SelectDown();
	void SelectLeft();
	void SelectRight();

	void ReorderSprites();

	void ShowMenu(wxTreeItemId id, const wxPoint& pt);

	void ChangeName(const SprPtr& spr);
	void Select(const SprPtr& spr, bool clear);
	void SelectSet(SpriteSelection* selection);
	void Reorder(const SprPtr& spr, bool up);
	bool Remove(const SprPtr& spr);

private:
	enum
	{
		ID_MENU_ADD = 100,
		ID_MENU_CLEAR,
		ID_MENU_VISIBLE,
		ID_MENU_EDITABLE,

		ID_GROUP_TREE_CTRL = 1000
	};

private:
	GroupTreePanel* m_parent_panel;

	MultiSpritesImpl* m_sprite_impl;

	wxTreeItemId m_root;
	wxTreeItemId m_on_menu_id;
	wxTreeItemId m_dragged_item;
	wxTreeItemId m_selected_item;

	bool m_add_del_open;

	const KeysState& m_key_state;

	bool m_expand_enable;

	bool m_select_enable;

	DECLARE_EVENT_TABLE()

}; // GroupTreeCtrl

}

#endif // _EASYEDITOR_GROUP_TREE_CTRL_H_
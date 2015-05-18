#ifndef _DRAG2D_GROUP_TREE_CTRL_H_
#define _DRAG2D_GROUP_TREE_CTRL_H_

#include <wx/treectrl.h>

namespace d2d
{

class ISprite;
class Group;
class MultiSpritesImpl;

class GroupTreeCtrl : public wxTreeCtrl
{
public:
	GroupTreeCtrl(wxWindow* parent, MultiSpritesImpl* sprite_impl);

	void AddNode();
	void DelNode();

	void Clear();

private:
	struct ItemData : wxTreeItemData
	{
		ItemData() : group(NULL), sprite(NULL) {}

		Group* group;

		ISprite* sprite;

	}; // ItemData

private:
	void AddNode(wxTreeItemId parent, const std::string& name, ItemData* data);

private:
	void OnItemRClick(wxTreeEvent& event);
	void OnItemMenu(wxTreeEvent& event);
	void OnItemActivated(wxTreeEvent& event);

	// menu
	void OnMenuAddSprites(wxCommandEvent& event);
	void OnMenuClear(wxCommandEvent& event);

	void ShowMenu(wxTreeItemId id, const wxPoint& pt);


private:
	enum
	{
		ID_ADD_SPRITES = 100,

		ID_GROUP_TREE_CTRL = 1000
	};

private:
	MultiSpritesImpl* m_sprite_impl;

	wxTreeItemId m_root;
	wxTreeItemId m_on_menu_id;

	DECLARE_EVENT_TABLE()

}; // GroupTreeCtrl

}

#endif // _DRAG2D_GROUP_TREE_CTRL_H_
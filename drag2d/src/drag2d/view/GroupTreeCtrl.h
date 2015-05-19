#ifndef _DRAG2D_GROUP_TREE_CTRL_H_
#define _DRAG2D_GROUP_TREE_CTRL_H_

#include <wx/treectrl.h>

#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class ISprite;
class Group;
class MultiSpritesImpl;

class GroupTreeCtrl : public wxTreeCtrl
{
public:
	class IVisitor
	{
	public:
		virtual void VisitNonleaf(wxTreeItemId id) {};
		virtual void VisitLeaf(wxTreeItemId id) {};
		virtual ~IVisitor() {}
	}; // IVisitor

public:
	GroupTreeCtrl(wxWindow* parent, MultiSpritesImpl* sprite_impl);

	// no break
	void Traverse(IVisitor& visitor);

	void AddNode();
	void DelNode();

	void Clear();

	void Remove(ISprite* sprite);

private:
	class ItemData;
	void AddNode(wxTreeItemId parent, const std::string& name, ItemData* data);

private:
	void OnItemRClick(wxTreeEvent& event);
	void OnItemMenu(wxTreeEvent& event);
	void OnItemActivated(wxTreeEvent& event);

	// menu
	void OnMenuAddSprites(wxCommandEvent& event);
	void OnMenuClear(wxCommandEvent& event);
	void OnMenuVisible(wxCommandEvent& event);
	void OnMenuEditable(wxCommandEvent& event);

	void ShowMenu(wxTreeItemId id, const wxPoint& pt);

	void Traverse(wxTreeItemId id, IVisitor& visitor);

private:
	class ItemData : public wxTreeItemData
	{
	public:
		ItemData(Group* group);
		ItemData(ISprite* sprite);
		virtual ~ItemData();

	public:
		Group* m_group;

		ISprite* m_sprite;

	}; // ItemData

	class RemoveVisitor : public IVisitor
	{
	public:
		RemoveVisitor(wxTreeCtrl* treectrl, d2d::ISprite* spr);
		virtual ~RemoveVisitor();

		virtual void VisitNonleaf(wxTreeItemId id);
		virtual void VisitLeaf(wxTreeItemId id);

	private:
		wxTreeCtrl* m_treectrl;

		d2d::ISprite* m_spr;

	}; // RemoveVisitor

	class SelectVisitor : public IVisitor
	{
	public:
		SelectVisitor(wxTreeCtrl* treectrl, SpriteSelection* selection);
		virtual ~SelectVisitor();

		virtual void VisitLeaf(wxTreeItemId id);

	private:
		wxTreeCtrl* m_treectrl;

		SpriteSelection* m_selection;

	}; // SelectVisitor

	class VisibleVisitor : public IVisitor
	{
	public:
		VisibleVisitor(wxTreeCtrl* treectrl) 
			: m_treectrl(treectrl) {}
		virtual void VisitLeaf(wxTreeItemId id);
	private:
		wxTreeCtrl* m_treectrl;
	}; // VisibleVisitor

	class EditableVisitor : public IVisitor
	{
	public:
		EditableVisitor(wxTreeCtrl* treectrl) 
			: m_treectrl(treectrl) {}
		virtual void VisitLeaf(wxTreeItemId id);
	private:
		wxTreeCtrl* m_treectrl;
	}; // EditableVisitor

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
	MultiSpritesImpl* m_sprite_impl;

	wxTreeItemId m_root;
	wxTreeItemId m_on_menu_id;

	DECLARE_EVENT_TABLE()

}; // GroupTreeCtrl

}

#endif // _DRAG2D_GROUP_TREE_CTRL_H_
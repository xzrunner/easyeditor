#ifndef _DRAG2D_GROUP_TREE_IMPL_H_
#define _DRAG2D_GROUP_TREE_IMPL_H_

#include "GroupTreeVisitor.h"

#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class ISprite;
class GroupTreeCtrl;

class GroupTreeImpl
{
public:
	class RemoveVisitor : public IGroupTreeVisitor
	{
	public:
		RemoveVisitor(wxTreeCtrl* treectrl, ISprite* spr);
		virtual ~RemoveVisitor();

		virtual void VisitNonleaf(wxTreeItemId id);
		virtual void VisitLeaf(wxTreeItemId id);

	private:
		wxTreeCtrl* m_treectrl;

		ISprite* m_spr;

	}; // RemoveVisitor

	class SelectVisitor : public IGroupTreeVisitor
	{
	public:
		SelectVisitor(wxTreeCtrl* treectrl, SpriteSelection* selection);
		virtual ~SelectVisitor();

		virtual void VisitLeaf(wxTreeItemId id);

	private:
		wxTreeCtrl* m_treectrl;

		SpriteSelection* m_selection;

	}; // SelectVisitor

	class VisibleVisitor : public IGroupTreeVisitor
	{
	public:
		VisibleVisitor(wxTreeCtrl* treectrl) 
			: m_treectrl(treectrl) {}
		virtual void VisitLeaf(wxTreeItemId id);
	private:
		wxTreeCtrl* m_treectrl;
	}; // VisibleVisitor

	class EditableVisitor : public IGroupTreeVisitor
	{
	public:
		EditableVisitor(wxTreeCtrl* treectrl) 
			: m_treectrl(treectrl) {}
		virtual void VisitLeaf(wxTreeItemId id);
	private:
		wxTreeCtrl* m_treectrl;
	}; // EditableVisitor	

	class SetVisibleVisitor : public IGroupTreeVisitor
	{
	public:
		SetVisibleVisitor(wxTreeCtrl* treectrl, bool visible) 
			: m_treectrl(treectrl), m_visible(visible) {}
		virtual void VisitLeaf(wxTreeItemId id);
	private:
		wxTreeCtrl* m_treectrl;
		bool m_visible;
	}; // SetVisibleVisitor

	class SetEditableVisitor : public IGroupTreeVisitor
	{
	public:
		SetEditableVisitor(wxTreeCtrl* treectrl, bool editable) 
			: m_treectrl(treectrl), m_editable(editable) {}
		virtual void VisitLeaf(wxTreeItemId id);
	private:
		wxTreeCtrl* m_treectrl;
		bool m_editable;
	}; // SetEditableVisitor	

	class StoreVisitor : public IGroupTreeVisitor
	{
	public:
		StoreVisitor(const GroupTreeCtrl* treectrl, Json::Value& value);

		virtual void VisitNonleaf(wxTreeItemId id);
		virtual void VisitLeaf(wxTreeItemId id);

	private:
		std::string GetName(wxTreeItemId id) const;
		std::string GetParentName(wxTreeItemId id) const;

	private:
		const GroupTreeCtrl* m_treectrl;
		Json::Value& m_value;

	}; // StoreVisitor

}; // GroupTreeImpl

}

#endif // _DRAG2D_GROUP_TREE_IMPL_H_
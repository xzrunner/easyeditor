#ifndef _DRAG2D_GROUP_TREE_IMPL_H_
#define _DRAG2D_GROUP_TREE_IMPL_H_

#include "GroupTreeVisitor.h"

#include "dataset/ObjSelectionSet.h"

namespace d2d
{

class ISprite;

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

	class StoreVisitor : public IGroupTreeVisitor
	{
	public:
		StoreVisitor(const wxTreeCtrl* treectrl, Json::Value& value);

		virtual void VisitNonleaf(wxTreeItemId id);
		virtual void VisitLeaf(wxTreeItemId id);

	private:
		const wxTreeCtrl* m_treectrl;
		Json::Value& m_value;

	}; // StoreVisitor

}; // GroupTreeImpl

}

#endif // _DRAG2D_GROUP_TREE_IMPL_H_
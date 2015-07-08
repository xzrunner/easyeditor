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
	class QuerySpriteVisitor : public IGroupTreeVisitor
	{
	public:
		QuerySpriteVisitor(wxTreeCtrl* treectrl, ISprite* spr);
		virtual ~QuerySpriteVisitor();

		virtual bool VisitLeaf(wxTreeItemId id);

		wxTreeItemId GetItemID() const { return m_id; }

	private:
		wxTreeCtrl* m_treectrl;

		ISprite* m_spr;

		wxTreeItemId m_id;

	}; // QuerySpriteVisitor

	class RemoveVisitor : public IGroupTreeVisitor
	{
	public:
		RemoveVisitor(wxTreeCtrl* treectrl, ISprite* spr);
		virtual ~RemoveVisitor();

		virtual bool VisitNonleaf(wxTreeItemId id);
		virtual bool VisitLeaf(wxTreeItemId id);

		bool IsFinish() const { return m_finish; }

	private:
		wxTreeCtrl* m_treectrl;

		ISprite* m_spr;

		bool m_finish;

	}; // RemoveVisitor

	class SelectVisitor : public IGroupTreeVisitor
	{
	public:
		SelectVisitor(wxTreeCtrl* treectrl, SpriteSelection* selection);
		virtual ~SelectVisitor();

		virtual bool VisitLeaf(wxTreeItemId id);

	private:
		wxTreeCtrl* m_treectrl;

		SpriteSelection* m_selection;

	}; // SelectVisitor

	class GetSpritesVisitor : public IGroupTreeVisitor
	{
	public:
		GetSpritesVisitor(wxTreeCtrl* treectrl, std::vector<ISprite*>& sprites);

		virtual bool VisitLeaf(wxTreeItemId id);

	private:
		wxTreeCtrl* m_treectrl;

		std::vector<ISprite*>& m_sprites;

	}; // GetSpritesVisitor

	class GetFirstSpriteVisitor : public IGroupTreeVisitor
	{
	public:
		GetFirstSpriteVisitor(wxTreeCtrl* treectrl);

		virtual bool VisitLeaf(wxTreeItemId id);

		ISprite* GetFirstSprite() { return m_first; }

	private:
		wxTreeCtrl* m_treectrl;

		ISprite* m_first;

	}; // GetFirstSpriteVisitor

	class VisibleVisitor : public IGroupTreeVisitor
	{
	public:
		VisibleVisitor(wxTreeCtrl* treectrl) 
			: m_treectrl(treectrl) {}
		virtual bool VisitLeaf(wxTreeItemId id);
	private:
		wxTreeCtrl* m_treectrl;
	}; // VisibleVisitor

	class EditableVisitor : public IGroupTreeVisitor
	{
	public:
		EditableVisitor(wxTreeCtrl* treectrl) 
			: m_treectrl(treectrl) {}
		virtual bool VisitLeaf(wxTreeItemId id);
	private:
		wxTreeCtrl* m_treectrl;
	}; // EditableVisitor	

	class SetVisibleVisitor : public IGroupTreeVisitor
	{
	public:
		SetVisibleVisitor(wxTreeCtrl* treectrl, bool visible) 
			: m_treectrl(treectrl), m_visible(visible) {}
		virtual bool VisitLeaf(wxTreeItemId id);
	private:
		wxTreeCtrl* m_treectrl;
		bool m_visible;
	}; // SetVisibleVisitor

	class SetEditableVisitor : public IGroupTreeVisitor
	{
	public:
		SetEditableVisitor(wxTreeCtrl* treectrl, bool editable) 
			: m_treectrl(treectrl), m_editable(editable) {}
		virtual bool VisitLeaf(wxTreeItemId id);
	private:
		wxTreeCtrl* m_treectrl;
		bool m_editable;
	}; // SetEditableVisitor	

	class StoreVisitor : public IGroupTreeVisitor
	{
	public:
		StoreVisitor(const GroupTreeCtrl* treectrl, Json::Value& value);

		virtual bool VisitNonleaf(wxTreeItemId id);
		virtual bool VisitLeaf(wxTreeItemId id);

	private:
		std::string GetName(wxTreeItemId id) const;
		std::string GetParentName(wxTreeItemId id) const;

	private:
		const GroupTreeCtrl* m_treectrl;
		Json::Value& m_value;

	}; // StoreVisitor

	class CopyPasteVisitor : public IGroupTreeVisitor
	{
	public:
		CopyPasteVisitor(GroupTreeCtrl* treectrl, wxTreeItemId from, 
			wxTreeItemId to);

		virtual bool VisitNonleaf(wxTreeItemId id);
		virtual bool VisitLeaf(wxTreeItemId id);

	private:
		void CopyPaste(wxTreeItemId id);

	private:
		GroupTreeCtrl* m_treectrl;
		wxTreeItemId m_from, m_to;

		std::map<wxTreeItemId, wxTreeItemId> m_map_ids;

	}; // StoreVisitor

}; // GroupTreeImpl

}

#endif // _DRAG2D_GROUP_TREE_IMPL_H_
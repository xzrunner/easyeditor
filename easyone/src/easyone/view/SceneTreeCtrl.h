#pragma once

#include "msg/Observer.h"

#include <node3/SceneNode.h>

#include <wx/treectrl.h>

namespace eone
{

class SceneTreePanel;
class SubjectMgr;

class SceneTreeCtrl : public wxTreeCtrl, public Observer
{
public:
	SceneTreeCtrl(wxWindow* parent, SubjectMgr& sub_mgr);

	virtual void OnNotify(MessageID msg, const VariantSet& variants) override;

	void Traverse(wxTreeItemId id, std::function<bool(wxTreeItemId)> func) const;

private:
	void InitRoot();

	void OnSelChanged(wxTreeEvent& event);
	//void OnSelChanging(wxTreeEvent& event);
	void OnLabelEdited(wxTreeEvent& event);

	void SelectSceneNode(const VariantSet& variants);
	void UnselectSceneNode(const VariantSet& variants);
	void InsertSceneNode(const VariantSet& variants);
	void InsertSceneNode(wxTreeItemId parent, const n3::SceneNodePtr& child);

private:
	enum
	{
		ID_SCENE_TREE_CTRL = 1000
	};

private:
	SubjectMgr& m_sub_mgr;

	wxTreeItemId m_root;

	DECLARE_EVENT_TABLE()

}; // SceneTreeCtrl

}
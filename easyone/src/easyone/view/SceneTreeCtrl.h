#pragma once

#include "data/SceneNode.h"
#include "msg/Observer.h"

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

private:
	void InsertSceneNode(const VariantSet& variants);

private:
	enum
	{
		//ID_MENU_ADD = 100,
		//ID_MENU_CLEAR,
		//ID_MENU_VISIBLE,
		//ID_MENU_EDITABLE,

		ID_GROUP_TREE_CTRL = 1000
	};

}; // SceneTreeCtrl

}
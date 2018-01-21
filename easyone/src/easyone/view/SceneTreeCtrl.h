#pragma once

#include "data/SceneNode.h"
#include "msg/Observer.h"

#include <wx/treectrl.h>

namespace eone
{

class SceneTreePanel;

class SceneTreeCtrl : public wxTreeCtrl, public Observer
{
public:
	SceneTreeCtrl(wxWindow* parent);

	virtual void OnNotify(int sub_id, const VariantSet& variants) override;

	void InsertNode(const SceneNodePtr& ndoe);

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
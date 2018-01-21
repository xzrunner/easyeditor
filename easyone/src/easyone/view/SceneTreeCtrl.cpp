#include "view/SceneTreeCtrl.h"
#include "msg/MessageID.h"

namespace eone
{

SceneTreeCtrl::SceneTreeCtrl(wxWindow* parent)
	: wxTreeCtrl(parent, ID_GROUP_TREE_CTRL, wxDefaultPosition, wxDefaultSize,
	wxTR_EDIT_LABELS/* | wxTR_MULTIPLE*/ | wxTR_NO_LINES | wxTR_DEFAULT_STYLE)
{
}

void SceneTreeCtrl::OnNotify(int sub_id, const VariantSet& variants)
{
	switch (sub_id)
	{
	case MSG_INSERT_NODE:
		break;
	}
}

void SceneTreeCtrl::InsertNode(const SceneNodePtr& ndoe)
{
	auto selection = GetSelection();
	if (selection.IsOk()) {
		
	} else {
		
	}
}

}
#include "view/SceneTreeCtrl.h"
#include "msg/MessageID.h"

namespace eone
{

SceneTreeCtrl::SceneTreeCtrl(wxWindow* parent)
	: wxTreeCtrl(parent, ID_GROUP_TREE_CTRL, wxDefaultPosition, wxDefaultSize,
	wxTR_EDIT_LABELS/* | wxTR_MULTIPLE*/ | wxTR_NO_LINES | wxTR_DEFAULT_STYLE)
{
}

void SceneTreeCtrl::OnNotify(MessageID msg, const VariantSet& variants)
{
	switch (msg)
	{
	case MSG_INSERT_SCENE_NODE:
		InsertSceneNode(variants);
		break;
	}
}

void SceneTreeCtrl::InsertSceneNode(const VariantSet& variants)
{
	auto selection = GetSelection();
	if (selection.IsOk()) {
			
	} else {
			
	}
}

}
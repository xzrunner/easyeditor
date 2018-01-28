#include "ee2/StagePanel.h"
#include "ee2/StageDropTarget.h"

namespace ee2
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
	                   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
{
	m_sub_mgr.RegisterObserver(ee0::MSG_INSERT_SCENE_NODE, this);
	m_sub_mgr.RegisterObserver(ee0::MSG_DELETE_SCENE_NODE, this);

	SetDropTarget(new StageDropTarget(this, library, this));
}

void StagePanel::OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants)
{
	switch (msg)
	{
	// scene node
	case ee0::MSG_INSERT_SCENE_NODE:
		InsertSceneNode(variants);
		break;
	case ee0::MSG_DELETE_SCENE_NODE:
		DeleteSceneNode(variants);
		break;
	}
}

void StagePanel::InsertSceneNode(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != VT_EMPTY, "no var in vars: node");
	n0::SceneNodePtr* node = static_cast<n0::SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");
	if (m_node_selection.IsEmpty()) {
		m_nodes.push_back(*node);
	}

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void StagePanel::DeleteSceneNode(const ee0::VariantSet& variants)
{
}

}
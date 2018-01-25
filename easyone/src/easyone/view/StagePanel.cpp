#include "view/StagePanel.h"
#include "view/StageCanvas.h"
#include "view/StageDropTarget.h"
#include "msg/VariantSet.h"

#include <guard/check.h>

namespace eone
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
	                   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
{
	m_sub_mgr.RegisterObserver(MSG_INSERT_SCENE_NODE, this);
	m_sub_mgr.RegisterObserver(MSG_DELETE_SCENE_NODE, this);
	m_sub_mgr.RegisterObserver(MSG_NODE_SELECTION_INSERT, this);
	m_sub_mgr.RegisterObserver(MSG_NODE_SELECTION_DELETE, this);
	m_sub_mgr.RegisterObserver(MSG_NODE_SELECTION_CLEAR, this);

	SetDropTarget(new StageDropTarget(this, library, this));
}

void StagePanel::OnNotify(MessageID msg, const VariantSet& variants)
{
	switch (msg)
	{
	// scene node
	case MSG_INSERT_SCENE_NODE:
		InsertSceneNode(variants);
		break;
	case MSG_DELETE_SCENE_NODE:
		DeleteSceneNode(variants);
		break;

	// selection set
	case MSG_NODE_SELECTION_INSERT:
		NodeSelectionInsert(variants);
		break;
	case MSG_NODE_SELECTION_DELETE:
		NodeSelectionDelete(variants);
		break;
	case MSG_NODE_SELECTION_CLEAR:
		m_node_selection.Clear();
		m_sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);
		break;
	}
}

sm::vec3 StagePanel::TransPosScrToProj3d(int x, int y) const
{
	auto canvas = std::dynamic_pointer_cast<const StageCanvas>(GetCanvas());
	if (!canvas) {
		return sm::vec3();
	}

	auto dir = canvas->TransPos3ScreenToDir(sm::vec2(x, y));
	auto& cam = canvas->GetCamera();
	sm::vec3 ret = dir.Normalized() * cam.GetDistance();
	ret.y = 0;
	return ret;
}

void StagePanel::InsertSceneNode(const VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != VT_EMPTY, "no var in vars: node");
	SceneNodePtr* node = static_cast<SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");
	if (m_node_selection.IsEmpty()) {
		m_nodes.push_back(*node);
	}

	m_sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

void StagePanel::DeleteSceneNode(const VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != VT_EMPTY, "no var in vars: node");
	SceneNodePtr* node = static_cast<SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");

	bool dirty = false;
	for (auto itr = m_nodes.begin(); itr != m_nodes.end(); ++itr) {
		if (*itr == *node) {
			dirty = true;
			m_nodes.erase(itr);
			break;
		}
	}
	
	if (dirty) {
		m_sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);
	}
}

void StagePanel::NodeSelectionInsert(const VariantSet& variants)
{
	auto var_clear = variants.GetVariant("clear");
	if (var_clear.m_type == VT_BOOL && var_clear.m_val.bl) {
		m_node_selection.Clear();
	}

	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != VT_EMPTY, "no var in vars: node");
	SceneNodePtr* node = static_cast<SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");

	if (m_node_selection.IsEmpty()) {
		m_sub_mgr.NotifyObservers(MSG_SELECTED_ONE_NODE, variants);
	}
	m_node_selection.Add(*node);

	m_sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

void StagePanel::NodeSelectionDelete(const VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != VT_EMPTY, "no var in vars: node");
	SceneNodePtr* node = static_cast<SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");

	m_node_selection.Remove(*node);

	m_sub_mgr.NotifyObservers(MSG_SET_CANVAS_DIRTY);
}

}
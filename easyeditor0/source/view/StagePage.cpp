#include "ee0/StagePage.h"

namespace ee0
{

StagePage::StagePage(wxWindow* parent, wxTopLevelWindow* frame)
	: ee::EditPanel(parent, frame) 
{
	m_sub_mgr.RegisterObserver(ee0::MSG_NODE_SELECTION_INSERT, this);
	m_sub_mgr.RegisterObserver(ee0::MSG_NODE_SELECTION_DELETE, this);
	m_sub_mgr.RegisterObserver(ee0::MSG_NODE_SELECTION_CLEAR, this);
}

void StagePage::OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants)
{
	switch (msg)
	{
	case ee0::MSG_NODE_SELECTION_INSERT:
		NodeSelectionInsert(variants);
		break;
	case ee0::MSG_NODE_SELECTION_DELETE:
		NodeSelectionDelete(variants);
		break;
	case ee0::MSG_NODE_SELECTION_CLEAR:
		m_node_selection.Clear();
		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
		break;
	}
}

void StagePage::NodeSelectionInsert(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != VT_EMPTY, "no var in vars: node");
	n0::SceneNodePtr* node = static_cast<n0::SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");

	if (m_node_selection.IsEmpty()) {
		m_sub_mgr.NotifyObservers(ee0::MSG_SELECTED_ONE_NODE, variants);
	}
	m_node_selection.Add(*node);

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void StagePage::NodeSelectionDelete(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != VT_EMPTY, "no var in vars: node");
	n0::SceneNodePtr* node = static_cast<n0::SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");

	m_node_selection.Remove(*node);

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}
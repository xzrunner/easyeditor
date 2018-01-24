#include "view/DetailPanel.h"
#include "msg/SubjectMgr.h"
#include "msg/VariantSet.h"

#include "view/CompTransformPanel.h"

#include <guard/check.h>

#include <wx/sizer.h>
#include <wx/button.h>

namespace eone
{

DetailPanel::DetailPanel(wxWindow* parent, SubjectMgr& sub_mgr)
	: wxPanel(parent, wxID_ANY)
	, m_sub_mgr(sub_mgr)
{
	InitLayout();

	sub_mgr.RegisterObserver(MSG_SELECTED_ONE_NODE, this);
	sub_mgr.RegisterObserver(MSG_NODE_SELECTION_CLEAR, this);
	sub_mgr.RegisterObserver(MSG_UPDATE_COMPONENTS, this);
}

void DetailPanel::OnNotify(MessageID msg, const VariantSet& variants)
{
	switch (msg)
	{
	case MSG_SELECTED_ONE_NODE:
		InitComponents(variants);
		break;
	case MSG_NODE_SELECTION_CLEAR:
		ClearComponents();
		break;
	case MSG_UPDATE_COMPONENTS:
		UpdateComponents();
		break;
	}
}

void DetailPanel::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	m_comp_sizer = new wxBoxSizer(wxVERTICAL);
	top_sizer->Add(m_comp_sizer);

	top_sizer->Add(new wxButton(this, wxID_ANY, "Add Component"));

	SetSizer(top_sizer);
}

void DetailPanel::InitComponents(const VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != VT_EMPTY, "no var in vars: node");
	GD_ASSERT(var.m_val.pv, "err scene node")
	m_node = *static_cast<SceneNodePtr*>(var.m_val.pv);

	if (m_node->HasComponent<n3::CompTransform>())
	{
		auto& comp = m_node->GetComponent<n3::CompTransform>();
		auto panel = new CompTransformPanel(this, comp, m_sub_mgr);
		m_comp_sizer->Insert(m_components.size(), panel);
		m_components.push_back(panel);
	}

	Layout();
}

void DetailPanel::ClearComponents()
{
	m_node.reset();
	if (m_children.empty()) {
		return;
	}
	for (int i = 0, n = m_components.size(); i < n; ++i) {
		m_comp_sizer->Detach(m_components[i]);
		delete m_components[i];
	}
	m_components.clear();

	Layout();
}

void DetailPanel::UpdateComponents()
{
	if (!m_node) {
		return;
	}
	for (auto& comp : m_components) {
		comp->RefreshNodeComp();
	}
}

}
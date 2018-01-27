#include "DetailPanel.h"
#include "CompTransformPanel.h"

#include <ee0/SubjectMgr.h>
#include <ee0/VariantSet.h>

#include <guard/check.h>

#include <wx/sizer.h>
#include <wx/button.h>

namespace eone
{

DetailPanel::DetailPanel(wxWindow* parent, ee0::SubjectMgr& sub_mgr)
	: wxPanel(parent, wxID_ANY)
	, m_sub_mgr(sub_mgr)
{
	SetBackgroundColour(wxColour(229, 229, 229));

	InitLayout();

	sub_mgr.RegisterObserver(ee0::MSG_SELECTED_ONE_NODE, this);
	sub_mgr.RegisterObserver(ee0::MSG_NODE_SELECTION_CLEAR, this);
	sub_mgr.RegisterObserver(ee0::MSG_UPDATE_COMPONENTS, this);
}

void DetailPanel::OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants)
{
	switch (msg)
	{
	case ee0::MSG_SELECTED_ONE_NODE:
		ClearComponents();
		InitComponents(variants);
		break;
	case ee0::MSG_NODE_SELECTION_CLEAR:
		ClearComponents();
		break;
	case ee0::MSG_UPDATE_COMPONENTS:
		UpdateComponents();
		break;
	}
}

void DetailPanel::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	m_comp_sizer = new wxBoxSizer(wxVERTICAL);
	top_sizer->Add(m_comp_sizer);

	top_sizer->AddSpacer(100);

	top_sizer->Add(new wxButton(this, wxID_ANY, "Add Component"), 0, wxALIGN_CENTER_HORIZONTAL);

	SetSizer(top_sizer);
}

void DetailPanel::InitComponents(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type != ee0::VT_EMPTY, "no var in vars: node");
	GD_ASSERT(var.m_val.pv, "err scene node")
	m_node = *static_cast<n3::SceneNodePtr*>(var.m_val.pv);

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
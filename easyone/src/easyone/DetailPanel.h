#pragma once

#include <ee0/Observer.h>

#include <node3/SceneNode.h>

#include <wx/panel.h>

namespace ee0 { class SubjectMgr; }

namespace eone
{
	
class NodeCompPanel;

class DetailPanel : public wxPanel, public ee0::Observer
{
public:
	DetailPanel(wxWindow* parent, ee0::SubjectMgr& sub_mgr);

	virtual void OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants) override;

private:
	void InitLayout();

	void InitComponents(const ee0::VariantSet& variants);
	void ClearComponents();
	void UpdateComponents();

private:
	ee0::SubjectMgr& m_sub_mgr;

	wxSizer* m_comp_sizer;

	std::vector<NodeCompPanel*> m_components;

	n3::SceneNodePtr m_node = nullptr;

}; // DetailPanel

}
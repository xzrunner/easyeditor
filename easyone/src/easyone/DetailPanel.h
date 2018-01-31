#pragma once

#include <ee0/Observer.h>

#include <node0/SceneNode.h>

#include <wx/panel.h>

namespace ee0 { class SubjectMgr; class NodeCompPanel; }

namespace eone
{

class DetailPanel : public wxPanel, public ee0::Observer
{
public:
	DetailPanel(wxWindow* parent, ee0::SubjectMgr& sub_mgr);

	virtual void OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants) override;

private:
	void InitLayout();
	void RegisterMsg(ee0::SubjectMgr& sub_mgr);

	void InitComponents(const ee0::VariantSet& variants);
	void ClearComponents();
	void UpdateComponents();
	void StagePageChanging(const ee0::VariantSet& variants);

private:
	ee0::SubjectMgr* m_sub_mgr;

	wxSizer* m_comp_sizer;

	std::vector<ee0::NodeCompPanel*> m_components;

	n0::SceneNodePtr m_node = nullptr;

}; // DetailPanel

}
#pragma once

#include "msg/Observer.h"
#include "data/SceneNode.h"

#include <wx/panel.h>

namespace eone
{
	
class SubjectMgr;
class NodeCompPanel;

class DetailPanel : public wxPanel, public Observer
{
public:
	DetailPanel(wxWindow* parent, SubjectMgr& sub_mgr);

	virtual void OnNotify(MessageID msg, const VariantSet& variants) override;

private:
	void InitLayout();

	void InitComponents(const VariantSet& variants);
	void ClearComponents();
	void UpdateComponents();

private:
	SubjectMgr& m_sub_mgr;

	wxSizer* m_comp_sizer;

	std::vector<NodeCompPanel*> m_components;

	SceneNodePtr m_node = nullptr;

}; // DetailPanel

}
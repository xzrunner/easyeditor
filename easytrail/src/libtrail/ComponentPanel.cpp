#include "ComponentPanel.h"
#include "ToolbarPanel.h"

#include <ee/Symbol.h>
#include <ee/SliderCtrl.h>

#include <mt_2d.h>

#include <wx/wx.h>

namespace etrail
{

ComponentPanel::ComponentPanel(wxWindow* parent, t2d_symbol* pc, ToolbarPanel* toolbar)	
	: wxPanel(parent)
	, m_pc(pc)
	, m_toolbar(toolbar) 
{}

void ComponentPanel::Load(const Json::Value& val)
{
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Load(val, 0);
		m_sliders[i]->Update();
	}
}

void ComponentPanel::Store(Json::Value& val) const
{
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Store(val);
	}
}

void ComponentPanel::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, GetTitle()); 
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	InitLayout(sizer);
	top_sizer->Add(sizer);

	top_sizer->AddSpacer(20);

	SetSizer(top_sizer);

	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Update();
	}
}

void ComponentPanel::OnDelete(wxCommandEvent& event)
{
	m_toolbar->OnDelChild(this);
}

}
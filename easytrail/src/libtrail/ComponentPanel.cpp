#include "ComponentPanel.h"
#include "ToolbarPanel.h"

#include <ee/Symbol.h>
#include <ee/FileHelper.h>
#include <ee/SliderCtrl.h>

#include <mt_2d.h>

#include <wx/wx.h>

namespace etrail
{

void ComponentPanel::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	std::string name = static_cast<ee::Symbol*>(m_pc->mode.B.ud)->GetFilepath();
	name = ee::FileHelper::GetFilename(name);

	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, name); 
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
#include "ComponentPanel.h"
#include "ToolBarPanel.h"
#include "language.h"
#include "ps_config.h"

#include <ee/Math2D.h>
#include <ee/Symbol.h>
#include <ee/FileHelper.h>
#include <ee/ImagePanel.h>
#include <ee/SliderCtrlTwo.h>
#include <ee/RGBColorSettingDlg.h>

namespace eparticle2d
{

ComponentPanel::ComponentPanel(wxWindow* parent, p2d_symbol* component, 
	ToolbarPanel* toolbar)
	: wxPanel(parent)
	, m_component(component)
	, m_toolbar(toolbar)
{
	InitLayout();
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Update();
	}
}

void ComponentPanel::SetValue(int key, const ee::UICallback::Data& data)
{
	switch (key)
	{
	case PS_ANGLE:
		m_component->angle_start = data.val0 * SM_DEG_TO_RAD;
		m_component->angle_end = data.val1 * SM_DEG_TO_RAD;
		break;
	case PS_SCALE:
		m_component->scale_start = data.val0 * 0.01f;
		m_component->scale_end = data.val1 * 0.01f;
		break;
	case PS_ALPHA:
		m_component->mul_col_begin.a = data.val0 / 100;
		m_component->mul_col_end.a = data.val1 / 100;
		break;
	}
}

void ComponentPanel::GetValue(int key, ee::UICallback::Data& data)
{
	switch (key)
	{
	case PS_ANGLE:
		data.val0 = m_component->angle_start * SM_RAD_TO_DEG;
		data.val1 = m_component->angle_end * SM_RAD_TO_DEG;
		break;
	case PS_SCALE:
		data.val0 = m_component->scale_start * 100;
		data.val1 = m_component->scale_end * 100;
		break;
	case PS_ALPHA:
		data.val0 = m_component->mul_col_begin.a * 100;
		data.val1 = m_component->mul_col_end.a * 100;
		break;
	}
}

void ComponentPanel::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	std::string name = static_cast<ee::Symbol*>(m_component->ud)->GetFilepath();
	name = ee::FileHelper::GetFilename(name);

	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, name); 
	wxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	InitLayout(sizer);
	top_sizer->Add(sizer);

	top_sizer->AddSpacer(20);

	SetSizer(top_sizer);
}

void ComponentPanel::InitLayout(wxSizer* top_sizer)
{
	// top
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		// Del
		{
			wxButton* btn = new wxButton(this, wxID_ANY, LANG[LK_REMOVE], wxDefaultPosition, wxSize(50, 50));
			Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnDelete));
			sizer->Add(btn);
		}
		sizer->AddSpacer(50);
		// Icon
		{
			std::string filepath = static_cast<ee::Symbol*>(m_component->ud)->GetFilepath();
			if (ee::FileType::IsType(filepath, ee::FileType::e_image)) {
				ee::ImagePanel* panel = new ee::ImagePanel(this, filepath, 100);
				sizer->Add(panel);
			}
			sizer->AddSpacer(10);
		}		
		top_sizer->Add(sizer);
	}
	// Angle
	{
		ee::SliderCtrlTwo* s_angle = new ee::SliderCtrlTwo(this, LANG[LK_ANGLE], "angle", this, PS_ANGLE, 
			ee::SliderItem(LANG[LK_START], "start", 0, -360, 360), ee::SliderItem(LANG[LK_END], "end", 0, -360, 360));
		top_sizer->Add(s_angle);
		top_sizer->AddSpacer(10);
		m_sliders.push_back(s_angle);	
	}
	// Scale
	{
		ee::SliderCtrlTwo* s_scale = new ee::SliderCtrlTwo(this, LANG[LK_SCALE], "scale", this, PS_SCALE, 
			ee::SliderItem(LANG[LK_START], "start", 100, 0, 500), ee::SliderItem(LANG[LK_END], "end", 100, 0, 500));
		top_sizer->Add(s_scale);
		top_sizer->AddSpacer(10);
		m_sliders.push_back(s_scale);	
	}
	// Multi Color
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		wxButton* start_btn = new wxButton(this, wxID_ANY, LANG[LK_COL_MUL_START]);
		Connect(start_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetMultiColStart));
		sizer->Add(start_btn);

		wxButton* end_btn = new wxButton(this, wxID_ANY, LANG[LK_COL_MUL_END]);
		Connect(end_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetMultiColEnd));
		sizer->Add(end_btn);

		top_sizer->Add(sizer);
	}
	// Add Color
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		wxButton* start_btn = new wxButton(this, wxID_ANY, LANG[LK_COL_ADD_START]);
		Connect(start_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetAddColStart));
		sizer->Add(start_btn);

		wxButton* end_btn = new wxButton(this, wxID_ANY, LANG[LK_COL_MUL_END]);
		Connect(end_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetAddColEnd));
		sizer->Add(end_btn);

		top_sizer->Add(sizer);
	}
	// Alpha
	{
		ee::SliderCtrlTwo* s_alpha = new ee::SliderCtrlTwo(this, LANG[LK_ALPHA], "alpha", this, PS_ALPHA, 
			ee::SliderItem(LANG[LK_START], "start", 100, 0, 100), ee::SliderItem(LANG[LK_END], "end", 100, 0, 100));
		top_sizer->Add(s_alpha);
		top_sizer->AddSpacer(10);
		m_sliders.push_back(s_alpha);
	}
}

void ComponentPanel::OnDelete(wxCommandEvent& event)
{
	m_toolbar->OnDelChild(this);
}

void ComponentPanel::OnSetMultiColStart(wxCommandEvent& event)
{
	UpdateColor(m_component->mul_col_begin);
}

void ComponentPanel::OnSetMultiColEnd(wxCommandEvent& event)
{
	UpdateColor(m_component->mul_col_end);
}

void ComponentPanel::OnSetAddColStart(wxCommandEvent& event)
{
	UpdateColor(m_component->add_col_begin);
}

void ComponentPanel::OnSetAddColEnd(wxCommandEvent& event)
{
	UpdateColor(m_component->add_col_end);
}

void ComponentPanel::UpdateColor(ps_color& color)
{
	s2::Color tmp;
	memcpy(&tmp.r, &color.r, sizeof(color));

	ee::RGBColorSettingDlg dlg(this, NULL, tmp);
	if (dlg.ShowModal() == wxID_OK) {
		tmp = dlg.GetColor();
		memcpy(&color.r, &tmp.r, sizeof(color));
	}
}

}
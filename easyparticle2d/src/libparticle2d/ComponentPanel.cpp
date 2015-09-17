#include "ComponentPanel.h"
#include "ToolBarPanel.h"
#include "language.h"
#include "ps_config.h"

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

void ComponentPanel::SetValue(int key, const d2d::UICallback::Data& data)
{
	switch (key)
	{
	case PS_ANGLE:
		m_component->angle_start = data.val0 * d2d::TRANS_DEG_TO_RAD;
		m_component->angle_end = data.val1 * d2d::TRANS_DEG_TO_RAD;
		break;
	case PS_SCALE:
		m_component->scale_start = data.val0 * 0.01f;
		m_component->scale_end = data.val1 * 0.01f;
		break;
	case PS_ALPHA:
		m_component->col_mul_start.a = data.val0;
		m_component->col_mul_end.a = data.val1;
		break;
	}
}

void ComponentPanel::GetValue(int key, d2d::UICallback::Data& data)
{
	switch (key)
	{
	case PS_ANGLE:
		data.val0 = m_component->angle_start * d2d::TRANS_RAD_TO_DEG;
		data.val1 = m_component->angle_end * d2d::TRANS_RAD_TO_DEG;
		break;
	case PS_SCALE:
		data.val0 = m_component->scale_start * 100;
		data.val1 = m_component->scale_end * 100;
		break;
	case PS_ALPHA:
		data.val0 = m_component->col_mul_start.a;
		data.val1 = m_component->col_mul_end.a;
		break;
	}
}

void ComponentPanel::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	std::string name = static_cast<d2d::ISymbol*>(m_component->ud)->GetFilepath();
	name = d2d::FilenameTools::getFilename(name);

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
			std::string filepath = static_cast<d2d::ISymbol*>(m_component->ud)->GetFilepath();
			if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image)) {
				d2d::ImagePanel* panel = new d2d::ImagePanel(this, filepath, 100);
				sizer->Add(panel);
			}
			sizer->AddSpacer(10);
		}		
		top_sizer->Add(sizer);
	}
	// Angle
	{
		d2d::SliderCtrlTwo* s_angle = new d2d::SliderCtrlTwo(this, LANG[LK_ANGLE], "angle", this, PS_ANGLE, 
			d2d::SliderItem(LANG[LK_START], "start", 0, -360, 360), d2d::SliderItem(LANG[LK_END], "end", 0, -360, 360));
		top_sizer->Add(s_angle);
		top_sizer->AddSpacer(10);
		m_sliders.push_back(s_angle);	
	}
	// Scale
	{
		d2d::SliderCtrlTwo* s_scale = new d2d::SliderCtrlTwo(this, LANG[LK_SCALE], "scale", this, PS_SCALE, 
			d2d::SliderItem(LANG[LK_START], "start", 100, 0, 500), d2d::SliderItem(LANG[LK_END], "end", 100, 0, 500));
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
		d2d::SliderCtrlTwo* s_alpha = new d2d::SliderCtrlTwo(this, LANG[LK_ALPHA], "alpha", this, PS_ALPHA, 
			d2d::SliderItem(LANG[LK_START], "start", 100, 0, 100), d2d::SliderItem(LANG[LK_END], "end", 100, 0, 100));
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
	UpdateColor(m_component->col_mul_start);
}

void ComponentPanel::OnSetMultiColEnd(wxCommandEvent& event)
{
	UpdateColor(m_component->col_mul_end);
}

void ComponentPanel::OnSetAddColStart(wxCommandEvent& event)
{
	UpdateColor(m_component->col_add_start);
}

void ComponentPanel::OnSetAddColEnd(wxCommandEvent& event)
{
	UpdateColor(m_component->col_add_end);
}

void ComponentPanel::UpdateColor(ps_color4f& color)
{
	d2d::Colorf tmp;
	memcpy(&tmp.r, &color.r, sizeof(color));

	d2d::RGBColorSettingDlg dlg(this, NULL, tmp);
	if (!dlg.ShowModal()) {
		return;
	}

	tmp = dlg.GetColor();
	memcpy(&color.r, &tmp.r, sizeof(color));
}

}
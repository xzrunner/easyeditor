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

#include <wx/colourdata.h>
#include <wx/colordlg.h>

namespace eparticle2d
{

ComponentPanel::ComponentPanel(wxWindow* parent, p2d_symbol* component, 
	ToolbarPanel* toolbar)
	: wxPanel(parent)
	, m_pc(component)
	, m_toolbar(toolbar)
{
	InitLayout();
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Update();
	}
	UpdateBtnColor();
}

void ComponentPanel::SetValue(int key, const ee::UICallback::Data& data)
{
	switch (key)
	{
	case PS_ANGLE:
		m_pc->angle_start = data.val0 * SM_DEG_TO_RAD;
		m_pc->angle_end = data.val1 * SM_DEG_TO_RAD;
		break;
	case PS_SCALE:
		m_pc->scale_start = data.val0 * 0.01f;
		m_pc->scale_end = data.val1 * 0.01f;
		break;
	case PS_ALPHA:
		m_pc->mul_col_begin.a = data.val0;
		m_pc->mul_col_end.a = data.val1;
		break;
	}
}

void ComponentPanel::GetValue(int key, ee::UICallback::Data& data)
{
	switch (key)
	{
	case PS_ANGLE:
		data.val0 = m_pc->angle_start * SM_RAD_TO_DEG;
		data.val1 = m_pc->angle_end * SM_RAD_TO_DEG;
		break;
	case PS_SCALE:
		data.val0 = m_pc->scale_start * 100;
		data.val1 = m_pc->scale_end * 100;
		break;
	case PS_ALPHA:
		data.val0 = m_pc->mul_col_begin.a;
		data.val1 = m_pc->mul_col_end.a;
		break;
	}
}

void ComponentPanel::UpdateBtnColor()
{
	m_begin_mul_col_btn->SetBackgroundColour(
		wxColour(m_pc->mul_col_begin.r, m_pc->mul_col_begin.g, m_pc->mul_col_begin.b));
	m_end_mul_col_btn->SetBackgroundColour(
		wxColour(m_pc->mul_col_end.r, m_pc->mul_col_end.g, m_pc->mul_col_end.b));
	m_begin_add_col_btn->SetBackgroundColour(
		wxColour(m_pc->add_col_begin.r, m_pc->add_col_begin.g, m_pc->add_col_begin.b));
	m_end_add_col_btn->SetBackgroundColour(
		wxColour(m_pc->add_col_end.r, m_pc->add_col_end.g, m_pc->add_col_end.b));
}

void ComponentPanel::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	std::string name = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(m_pc->ud))->GetFilepath();
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
			std::string filepath = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(m_pc->ud))->GetFilepath();
			if (ee::FileType::IsType(filepath, ee::FILE_IMAGE)) {
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
	// Mul Color
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "乘色"); 
		wxSizer* hori_sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		hori_sizer->Add(new wxStaticText(this, wxID_ANY, "起始"));
		hori_sizer->AddSpacer(5);
		m_begin_mul_col_btn = new wxButton(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, 20));
		Connect(m_begin_mul_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetBeginMulCol));
		hori_sizer->Add(m_begin_mul_col_btn);

		hori_sizer->AddSpacer(20);

		hori_sizer->Add(new wxStaticText(this, wxID_ANY, "结束"));
		hori_sizer->AddSpacer(5);
		m_end_mul_col_btn = new wxButton(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, 20));
		Connect(m_end_mul_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetEndMulCol));
		hori_sizer->Add(m_end_mul_col_btn);

		top_sizer->Add(hori_sizer);
	}
	// Add Color
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "加色"); 
		wxSizer* hori_sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		hori_sizer->Add(new wxStaticText(this, wxID_ANY, "起始"));
		hori_sizer->AddSpacer(5);
		m_begin_add_col_btn = new wxButton(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, 20));
		Connect(m_begin_add_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetBeginAddCol));
		hori_sizer->Add(m_begin_add_col_btn);

		hori_sizer->AddSpacer(20);

		hori_sizer->Add(new wxStaticText(this, wxID_ANY, "结束"));
		hori_sizer->AddSpacer(5);
		m_end_add_col_btn = new wxButton(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, 20));
		Connect(m_end_add_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetEndAddCol));
		hori_sizer->Add(m_end_add_col_btn);

		top_sizer->Add(hori_sizer);
	}
	// Alpha
	{
		ee::SliderCtrlTwo* s_alpha = new ee::SliderCtrlTwo(this, LANG[LK_ALPHA], "alpha", this, PS_ALPHA, 
			ee::SliderItem(LANG[LK_START], "start", 255, 0, 255), ee::SliderItem(LANG[LK_END], "end", 255, 0, 255));
		top_sizer->Add(s_alpha);
		top_sizer->AddSpacer(10);
		m_sliders.push_back(s_alpha);
	}
}

void ComponentPanel::OnDelete(wxCommandEvent& event)
{
	m_toolbar->OnDelChild(this);
}

void ComponentPanel::OnSetBeginMulCol(wxCommandEvent& event)
{
	ChangeColor(m_pc->mul_col_begin.rgba);
}

void ComponentPanel::OnSetEndMulCol(wxCommandEvent& event)
{
	ChangeColor(m_pc->mul_col_end.rgba);
}

void ComponentPanel::OnSetBeginAddCol(wxCommandEvent& event)
{
	ChangeColor(m_pc->add_col_begin.rgba);
}

void ComponentPanel::OnSetEndAddCol(wxCommandEvent& event)
{
	ChangeColor(m_pc->add_col_end.rgba);
}

void ComponentPanel::ChangeColor(uint8_t rgba[4])
{
	wxColourData data;
	data.SetColour(wxColour(rgba[0], rgba[1], rgba[2]));
	wxColourDialog dlg(m_parent, &data);

	dlg.SetTitle(wxT("Set Color"));

	wxPoint pos = wxGetMousePosition();
	pos.x -= 400;
	dlg.SetPosition(pos);

	if (dlg.ShowModal() == wxID_OK)
	{
		const wxColor& col = dlg.GetColourData().GetColour();
		rgba[0] = col.Red();
		rgba[1] = col.Green();
		rgba[2] = col.Blue();
		UpdateBtnColor();
	}
}

}
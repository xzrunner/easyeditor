#include "ComponentPanel.h"
#include "ToolBarPanel.h"
#include "ps_config.h"
#include "PSConfigMgr.h"
#include "language.h"

#include <ee/SliderCtrl.h>
#include <ee/Math2D.h>
#include <ee/Symbol.h>
#include <ee/FileHelper.h>
#include <ee/ImagePanel.h>
#include <ee/SliderCtrlTwo.h>

#include <ps_3d.h>

#include <wx/colourdata.h>
#include <wx/colordlg.h>
#include <wx/spinctrl.h>

namespace eparticle3d
{

static const float SCALE_START			= 100;
static const float SCALE_END			= 100;
static const float ROTATE_MIN			= 0;
static const float ROTATE_MAX			= 0;

ComponentPanel::ComponentPanel(wxWindow* parent, p3d_symbol* pc, ToolbarPanel* toolbar)
	: wxPanel(parent)
	, m_pc(pc)
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
	case PS_SCALE:
		m_pc->scale_start = data.val0 * 0.01f;
		m_pc->scale_end = data.val1 * 0.01f;
		break;
	case PS_ROTATE:
		m_pc->angle = (data.val0 + data.val1) * 0.5f * SM_DEG_TO_RAD;
		m_pc->angle_var = (data.val1 - data.val0) * 0.5f * SM_DEG_TO_RAD;
		break;
	case PS_ALPHA:
		m_pc->alpha_start = data.val0 * 0.01f;
		m_pc->alpha_end = data.val1 * 0.01f;
		break;
	}
}

void ComponentPanel::GetValue(int key, ee::UICallback::Data& data)
{
	switch (key)
	{
	case PS_SCALE:
		data.val0 = m_pc->scale_start * 100;
		data.val1 = m_pc->scale_end * 100;
		break;
	case PS_ROTATE:
		data.val0 = (m_pc->angle + m_pc->angle_var) * SM_RAD_TO_DEG;
		data.val1 = (m_pc->angle - m_pc->angle_var) * SM_RAD_TO_DEG;
		break;
	case PS_ALPHA:
		data.val0 = m_pc->alpha_start * 100;
		data.val1 = m_pc->alpha_end * 100;
		break;
	}
}

const ps_color4f& ComponentPanel::GetMulColor() const
{
	return m_pc->col_mul;
}

const ps_color4f& ComponentPanel::GetAddColor() const
{
	return m_pc->col_add;
}

void ComponentPanel::SetBtnColor()
{
	m_mul_col_btn->SetBackgroundColour(wxColour(m_pc->col_mul.r * 255, m_pc->col_mul.g * 255, m_pc->col_mul.b * 255));
	m_add_col_btn->SetBackgroundColour(wxColour(m_pc->col_add.r * 255, m_pc->col_add.g * 255, m_pc->col_add.b * 255));
}

void ComponentPanel::SetCount(int count)
{
	m_pc->count = count;
	m_count->SetValue(count);
}

void ComponentPanel::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	std::string name = static_cast<ee::Symbol*>(m_pc->ud)->GetFilepath();
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
		wxSizer* hori_sizer = new wxBoxSizer(wxHORIZONTAL);
		// Left
		{
			wxSizer* vert_sizer = new wxBoxSizer(wxVERTICAL);
			// Del
			{
//				wxButton* btn = new wxButton(this, wxID_ANY, LANG[LK_REMOVE], wxDefaultPosition, wxSize(50, 50));
				wxButton* btn = new wxButton(this, wxID_ANY, LANG[LK_REMOVE]);
				Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnDelete));
				vert_sizer->Add(btn);
			}
			vert_sizer->AddSpacer(8);
			// Name
			{
				wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
				sz->Add(new wxStaticText(this, wxID_ANY, LANG[LK_NAME]));
				sz->Add(m_name = new wxTextCtrl(this, wxID_ANY));
				vert_sizer->Add(sz);
			}
			vert_sizer->AddSpacer(8);
			// Count
			{
				wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
				sz->Add(new wxStaticText(this, wxID_ANY, LANG[LK_CONST_COUNT]));

				m_count = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS, 0, 100, 0);
				Connect(m_count->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ComponentPanel::OnSetCount));
				sz->Add(m_count);

				vert_sizer->Add(sz);
			}
			hori_sizer->Add(vert_sizer);
		}
		hori_sizer->AddSpacer(20);
		// Right Icon
		{
			std::string filepath = static_cast<ee::Symbol*>(m_pc->ud)->GetFilepath();
			if (ee::FileType::IsType(filepath, ee::FileType::e_image)) {
				ee::ImagePanel* panel = new ee::ImagePanel(this, filepath, 100);
				hori_sizer->Add(panel);
			}
		}	
		top_sizer->Add(hori_sizer);
	}
	// Scale
	ee::SliderCtrlTwo* s_scale = new ee::SliderCtrlTwo(this, LANG[LK_SCALE], "scale", this, PS_SCALE, 
		ee::SliderItem(LANG[LK_START], "start", SCALE_START, 0, 2000), ee::SliderItem(LANG[LK_END], "end", SCALE_END, 0, 2000));
	top_sizer->Add(s_scale);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_scale);
	// Rotate
	ee::SliderCtrlTwo* s_rotate = new ee::SliderCtrlTwo(this, LANG[LK_ROTATE], "rotate", this, PS_ROTATE, 
		ee::SliderItem(LANG[LK_MIN], "min", ROTATE_MIN, -180, 180), ee::SliderItem(LANG[LK_MAX], "max", ROTATE_MAX, -180, 180));
	top_sizer->Add(s_rotate);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_rotate);
	// Color
	wxBoxSizer* color_sz = new wxBoxSizer(wxHORIZONTAL);
	// Multi Color
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, LANG[LK_COL_MUL]);
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_mul_col_btn = new wxButton(this, wxID_ANY);
		Connect(m_mul_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetMultiCol));
		sizer->Add(m_mul_col_btn);

		color_sz->Add(sizer);
	}
	color_sz->AddSpacer(30);
	// Add Color
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, LANG[LK_COL_ADD]);
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_add_col_btn = new wxButton(this, wxID_ANY);
		Connect(m_add_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetAddCol));
		sizer->Add(m_add_col_btn);

		color_sz->Add(sizer);
	}
	top_sizer->Add(color_sz);
	// Alpha
	ee::SliderCtrlTwo* s_alpha = new ee::SliderCtrlTwo(this, LANG[LK_ALPHA], "alpha", this, PS_ALPHA, 
		ee::SliderItem(LANG[LK_START], "start", 100, 0, 100), ee::SliderItem(LANG[LK_END], "end", 100, 0, 100));
	top_sizer->Add(s_alpha);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_alpha);
	// Bind PS
	{
		wxButton* btn = new wxButton(this, wxID_ANY, LANG[LK_BIND_PS]);
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnBindPS));
		top_sizer->Add(btn);
	}
}

void ComponentPanel::OnDelete(wxCommandEvent& event)
{
	m_toolbar->OnDelChild(this);
}

void ComponentPanel::OnSetCount(wxSpinEvent& event)
{
	m_pc->count = event.GetValue();
}

void ComponentPanel::OnBindPS(wxCommandEvent& event)
{
	std::string filter = ee::FileType::GetTag(ee::FileType::e_particle3d);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("����Particle3D�ļ�"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_pc->bind_ps_cfg = PSConfigMgr::Instance()->GetConfig(dlg.GetPath().ToStdString());
	}
}

//void ComponentPanel::OnSetMultiCol(wxCommandEvent& event)
//{
//	ee::Colorf col;
//	memcpy(&col.r, &m_pc->col_mul.r, sizeof(m_pc->col_mul));
//
//	ee::RGBColorSettingDlg dlg(this, NULL, col);
//	if (!dlg.ShowModal() == wxID_OK) {
//		return;
//	}
//
//	col = dlg.GetColor();
//	memcpy(&m_pc->col_mul.r, &col.r, sizeof(m_pc->col_mul));
//}
//
//void ComponentPanel::OnSetAddCol(wxCommandEvent& event)
//{
//	ee::Colorf col;
//	memcpy(&col.r, &m_pc->col_add.r, sizeof(m_pc->col_add));
//
//	ee::RGBColorSettingDlg dlg(this, NULL, col);
//	if (!dlg.ShowModal() == wxID_OK) {
//		return;
//	}
//
//	col = dlg.GetColor();
//	memcpy(&m_pc->col_add.r, &col.r, sizeof(m_pc->col_add));
//}

void ComponentPanel::OnSetMultiCol(wxCommandEvent& event)
{
	wxColourData data;
	data.SetColour(wxColour(m_pc->col_mul.r * 255, m_pc->col_mul.g * 255, m_pc->col_mul.b * 255));
	wxColourDialog dlg(m_parent, &data);

	dlg.SetTitle(wxT("Multi Color"));

	wxPoint pos = wxGetMousePosition();
	pos.x -= 400;
	dlg.SetPosition(pos);

	if (dlg.ShowModal() == wxID_OK)
	{
		const wxColor& col = dlg.GetColourData().GetColour();
		m_pc->col_mul.r = col.Red() / 255.0f;
		m_pc->col_mul.g = col.Green() / 255.0f;
		m_pc->col_mul.b = col.Blue() / 255.0f;
		m_pc->col_mul.a = col.Alpha() / 255.0f;

		SetBtnColor();
	}
}

void ComponentPanel::OnSetAddCol(wxCommandEvent& event)
{
	wxColourData data;
	data.SetColour(wxColour(m_pc->col_add.r * 255, m_pc->col_add.g * 255, m_pc->col_add.b * 255));
	wxColourDialog dlg(m_parent, &data);

	dlg.SetTitle(wxT("Add Color"));

	wxPoint pos = wxGetMousePosition();
	pos.x -= 400;
	dlg.SetPosition(pos);

	if (dlg.ShowModal() == wxID_OK)
	{
		const wxColor& col = dlg.GetColourData().GetColour();
		m_pc->col_add.r = col.Red() / 255.0f;
		m_pc->col_add.g = col.Green() / 255.0f;
		m_pc->col_add.b = col.Blue() / 255.0f;
		m_pc->col_add.a = col.Alpha() / 255.0f;

		SetBtnColor();
	}
}

}
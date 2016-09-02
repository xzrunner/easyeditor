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
	UpdateBtnColor();
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
		m_pc->mul_col_begin.a = data.val0;
		m_pc->mul_col_end.a = data.val1;
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

void ComponentPanel::SetCount(int count)
{
	m_pc->count = count;
	m_count->SetValue(count);
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
			std::string filepath = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(m_pc->ud))->GetFilepath();
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
	ee::SliderCtrlTwo* s_alpha = new ee::SliderCtrlTwo(this, LANG[LK_ALPHA], "alpha2", this, PS_ALPHA, 
	ee::SliderItem(LANG[LK_START], "start", 255, 0, 255), ee::SliderItem(LANG[LK_END], "end", 255, 0, 255));
	top_sizer->Add(s_alpha);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_alpha);

//	wxBoxSizer* color_sz = new wxBoxSizer(wxHORIZONTAL);
// 	// Multi Color
// 	{
// 		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, LANG[LK_COL_MUL]);
// 		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
// 
// 		m_mul_col_btn = new wxButton(this, wxID_ANY);
// 		Connect(m_mul_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetMultiCol));
// 		sizer->Add(m_mul_col_btn);
// 
// 		color_sz->Add(sizer);
// 	}
// 	color_sz->AddSpacer(30);
// 	// Add Color
// 	{
// 		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, LANG[LK_COL_ADD]);
// 		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
// 
// 		m_add_col_btn = new wxButton(this, wxID_ANY);
// 		Connect(m_add_col_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetAddCol));
// 		sizer->Add(m_add_col_btn);
// 
// 		color_sz->Add(sizer);
// 	}
// 	top_sizer->Add(color_sz);
// 	// Alpha
// 	ee::SliderCtrlTwo* s_alpha = new ee::SliderCtrlTwo(this, LANG[LK_ALPHA], "alpha", this, PS_ALPHA, 
// 		ee::SliderItem(LANG[LK_START], "start", 100, 0, 100), ee::SliderItem(LANG[LK_END], "end", 100, 0, 100));
// 	top_sizer->Add(s_alpha);
// 	top_sizer->AddSpacer(10);
// 	m_sliders.push_back(s_alpha);
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
	wxFileDialog dlg(this, wxT("导入Particle3D文件"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_pc->bind_ps_cfg = PSConfigMgr::Instance()->GetConfig(dlg.GetPath().ToStdString());
	}
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
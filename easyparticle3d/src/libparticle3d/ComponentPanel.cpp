#include "ComponentPanel.h"
#include "ToolBarPanel.h"
#include "ps_config.h"
#include "PSConfigMgr.h"
#include "language.h"

#include <particle3d.h>

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

void ComponentPanel::SetValue(int key, const d2d::UICallback::Data& data)
{
	switch (key)
	{
	case PS_SCALE:
		m_pc->scale_start = data.val0 * 0.01f;
		m_pc->scale_end = data.val1 * 0.01f;
		break;
	case PS_ROTATE:
		m_pc->angle = (data.val0 + data.val1) * 0.5f * d2d::TRANS_DEG_TO_RAD;
		m_pc->angle_var = (data.val1 - data.val0) * 0.5f * d2d::TRANS_DEG_TO_RAD;
		break;
	case PS_ALPHA:
		m_pc->alpha_start = data.val0 * 0.01f;
		m_pc->alpha_end = data.val1 * 0.01f;
		break;
	}
}

void ComponentPanel::GetValue(int key, d2d::UICallback::Data& data)
{
	switch (key)
	{
	case PS_SCALE:
		data.val0 = m_pc->scale_start * 100;
		data.val1 = m_pc->scale_end * 100;
		break;
	case PS_ROTATE:
		data.val0 = (m_pc->angle + m_pc->angle_var) * d2d::TRANS_RAD_TO_DEG;
		data.val1 = (m_pc->angle - m_pc->angle_var) * d2d::TRANS_RAD_TO_DEG;
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

void ComponentPanel::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	std::string name = static_cast<d2d::ISymbol*>(m_pc->ud)->GetFilepath();
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
		wxSizer* hori_sizer = new wxBoxSizer(wxHORIZONTAL);
		// Left
		{
			wxSizer* vert_sizer = new wxBoxSizer(wxVERTICAL);
			// Del
			{
				wxButton* btn = new wxButton(this, wxID_ANY, LANG[LK_REMOVE], wxDefaultPosition, wxSize(50, 50));
				Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnDelete));
				vert_sizer->Add(btn);
			}
			vert_sizer->AddSpacer(20);
			// Name
			{
				wxSizer* sz = new wxBoxSizer(wxHORIZONTAL);
				sz->Add(new wxStaticText(this, wxID_ANY, LANG[LK_NAME]));
				sz->Add(m_name = new wxTextCtrl(this, wxID_ANY));
				vert_sizer->Add(sz);
			}
			hori_sizer->Add(vert_sizer);
		}
		hori_sizer->AddSpacer(20);
		// Right Icon
		{
			std::string filepath = static_cast<d2d::ISymbol*>(m_pc->ud)->GetFilepath();
			if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_image)) {
				d2d::ImagePanel* panel = new d2d::ImagePanel(this, filepath, 100);
				hori_sizer->Add(panel);
			}
		}	
		top_sizer->Add(hori_sizer);
	}
	// Scale
	d2d::SliderCtrlTwo* s_scale = new d2d::SliderCtrlTwo(this, LANG[LK_SCALE], "scale", this, PS_SCALE, 
		d2d::SliderItem(LANG[LK_START], "start", SCALE_START, 0, 2000), d2d::SliderItem(LANG[LK_END], "end", SCALE_END, 0, 2000));
	top_sizer->Add(s_scale);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_scale);
	// Rotate
	d2d::SliderCtrlTwo* s_rotate = new d2d::SliderCtrlTwo(this, LANG[LK_ROTATE], "rotate", this, PS_ROTATE, 
		d2d::SliderItem(LANG[LK_MIN], "min", ROTATE_MIN, -180, 180), d2d::SliderItem(LANG[LK_MAX], "max", ROTATE_MAX, -180, 180));
	top_sizer->Add(s_rotate);
	top_sizer->AddSpacer(10);
	m_sliders.push_back(s_rotate);
	// Multi Color
	wxButton* mul_btn = new wxButton(this, wxID_ANY, LANG[LK_COL_MUL]);
	Connect(mul_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetMultiCol));
	top_sizer->Add(mul_btn);
	// Add Color
	wxButton* add_btn = new wxButton(this, wxID_ANY, LANG[LK_COL_ADD]);
	Connect(add_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ComponentPanel::OnSetAddCol));
	top_sizer->Add(add_btn);
	// Alpha
	d2d::SliderCtrlTwo* s_alpha = new d2d::SliderCtrlTwo(this, LANG[LK_ALPHA], "alpha", this, PS_ALPHA, 
		d2d::SliderItem(LANG[LK_START], "start", 100, 0, 100), d2d::SliderItem(LANG[LK_END], "end", 100, 0, 100));
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

void ComponentPanel::OnBindPS(wxCommandEvent& event)
{
	wxString filter = d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_particle3d);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("导入Particle3D文件"), wxEmptyString, wxEmptyString, filter, wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		m_pc->bind_ps_cfg = PSConfigMgr::Instance()->GetConfig(dlg.GetPath().ToStdString());

		// 		if (m_canvas) {
		// 			m_canvas->ResetViewport();
		// 		}
	}
}

void ComponentPanel::OnSetMultiCol(wxCommandEvent& event)
{
	d2d::Colorf col;
	memcpy(&col.r, &m_pc->col_mul.r, sizeof(m_pc->col_mul));

	d2d::RGBColorSettingDlg dlg(this, NULL, col);
	if (!dlg.ShowModal()) {
		return;
	}

	col = dlg.GetColor();
	memcpy(&m_pc->col_mul.r, &col.r, sizeof(m_pc->col_mul));
}

void ComponentPanel::OnSetAddCol(wxCommandEvent& event)
{
	d2d::Colorf col;
	memcpy(&col.r, &m_pc->col_add.r, sizeof(m_pc->col_add));

	d2d::RGBColorSettingDlg dlg(this, NULL, col);
	if (!dlg.ShowModal()) {
		return;
	}

	col = dlg.GetColor();
	memcpy(&m_pc->col_add.r, &col.r, sizeof(m_pc->col_add));
}

}
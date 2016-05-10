#include "HSLColorSettingDlg.h"
#include "ColorSlider.h"
#include "convert_color.h"

namespace ee
{

HSLColorSettingDlg::HSLColorSettingDlg(wxWindow* parent, ColorMonitor* lsn, const s2::Color& col, const wxPoint& pos)
	: wxDialog(parent, wxID_ANY, "HSL Color Setting", pos, wxSize(450, 300))
	, m_lsn(lsn)
	, m_h_ctrl(NULL)
	, m_s_ctrl(NULL)
	, m_l_ctrl(NULL)
{
	m_h = m_s = m_l = 0;

	InitLayout();
	SetColor(col);
}

s2::Color HSLColorSettingDlg::GetColor() const
{
	float r, g, b;
	hsl2rgb(m_h, m_s, m_l, r, g, b);
	return s2::Color((int)(r * 255 + 0.5f), (int)(g * 255 + 0.5f), (int)(b * 255 + 0.5f));
}

void HSLColorSettingDlg::OnColorChanged()
{
	int h = m_h_ctrl->GetColorValue(),
		s = m_s_ctrl->GetColorValue(),
		l = m_l_ctrl->GetColorValue();
	m_h = h / 255.0f;
	m_s = s / 255.0f;
	m_l = l / 255.0f;

	m_h_ctrl->SetColorRegion(s2::Color(0, s, l), s2::Color(255, s, l));
	m_s_ctrl->SetColorRegion(s2::Color(h, 0, l), s2::Color(h, 255, l));
	m_l_ctrl->SetColorRegion(s2::Color(h, s, 0), s2::Color(h, s, 255));

	float r, g, b;
	hsl2rgb(m_h, m_s, m_l, r, g, b);
	m_color_bg->SetBackgroundColour(wxColour(r * 255 + 0.5f, g * 255 + 0.5f, b * 255 + 0.5f));
	m_color_bg->Refresh(true);

	if (m_lsn) {
		m_lsn->OnColorChanged(s2::Color((int)(r * 255 + 0.5f), (int)(g * 255 + 0.5f), (int)(b * 255 + 0.5f)));
	}
}

void HSLColorSettingDlg::OnColorChanged(const s2::Color& col)
{
	OnColorChanged();
}

void HSLColorSettingDlg::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	// color
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "color");
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		m_color_bg = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(400, 100));
		sizer->Add(m_color_bg);
		top_sizer->Add(sizer);
	}
	// setting
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "setting");
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_h_ctrl = new ColorSlider(this, this, "色相", false);
		sizer->Add(m_h_ctrl);
		sizer->AddSpacer(10);
		m_s_ctrl = new ColorSlider(this, this, "饱和度", false);
		sizer->Add(m_s_ctrl);
		sizer->AddSpacer(10);
		m_l_ctrl = new ColorSlider(this, this, "亮度", false);
		sizer->Add(m_l_ctrl);

		top_sizer->Add(sizer);
	}
	// default
	{
		SetEscapeId(wxID_CANCEL);
		top_sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxCENTER);
	}
	SetSizer(top_sizer);
	top_sizer->Fit(this);
}

void HSLColorSettingDlg::SetColor(const s2::Color& col)
{
	rgb2hsl(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, m_h, m_s, m_l);

	int ih = (int)(m_h * 255 + 0.5f),
		is = (int)(m_s * 255 + 0.5f),
		il = (int)(m_l * 255 + 0.5f);
	m_h_ctrl->SetColorValue(ih);
	m_s_ctrl->SetColorValue(is);
	m_l_ctrl->SetColorValue(il);
	m_h_ctrl->SetColorRegion(s2::Color(0, is, il), s2::Color(255, is, il));
	m_s_ctrl->SetColorRegion(s2::Color(ih, 0, il), s2::Color(ih, 255, il));
	m_l_ctrl->SetColorRegion(s2::Color(ih, is, 0), s2::Color(ih, is, 255));

	m_color_bg->SetBackgroundColour(wxColour(col.r, col.g, col.b));
	m_color_bg->Refresh(true);

	if (m_lsn) {
		m_lsn->OnColorChanged(col);
	}
}

}
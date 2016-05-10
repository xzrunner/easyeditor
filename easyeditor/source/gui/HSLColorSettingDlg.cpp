#include "HSLColorSettingDlg.h"
#include "ColorSlider.h"
#include "convert_color.h"

namespace ee
{

HSLColorSettingDlg::HSLColorSettingDlg(wxWindow* parent, ColorMonitor* lsn, const s2::Color& col, const wxPoint& pos)
	: wxDialog(parent, wxID_ANY, "HSL Color Setting", pos, wxSize(450, 300))
	, m_lsn(lsn)
	, m_h(NULL)
	, m_s(NULL)
	, m_l(NULL)
{
	InitLayout();
	SetColor(col);
}

s2::Color HSLColorSettingDlg::GetColor() const
{
	float r, g, b;
	hsl2rgb(m_h->GetColorValue() / 255.0f, m_s->GetColorValue() / 255.0f, m_l->GetColorValue() / 255.0f, r, g, b);
	return s2::Color((int)(r * 255 + 0.5f), (int)(g * 255 + 0.5f), (int)(b * 255 + 0.5f));
}

void HSLColorSettingDlg::OnColorChanged()
{
	int h = m_h->GetColorValue();
	int s = m_s->GetColorValue();
	int l = m_l->GetColorValue();
	m_h->SetColorRegion(s2::Color(0, s, l), s2::Color(255, s, l));
	m_s->SetColorRegion(s2::Color(h, 0, l), s2::Color(h, 255, l));
	m_l->SetColorRegion(s2::Color(h, s, 0), s2::Color(h, s, 255));

	float r, g, b;
	hsl2rgb(m_h->GetColorValue() / 255.0f, m_s->GetColorValue() / 255.0f, m_l->GetColorValue() / 255.0f, r, g, b);
	m_color_bg->SetBackgroundColour(wxColour(r * 255 + 0.5f, g * 255 + 0.5f, b * 255 + 0.5f));
	m_color_bg->Refresh(true);

	if (m_lsn) {
		m_lsn->OnColorChanged(GetColor());
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

		m_h = new ColorSlider(this, this, "色相", false);
		sizer->Add(m_h);
		sizer->AddSpacer(10);
		m_s = new ColorSlider(this, this, "饱和度", false);
		sizer->Add(m_s);
		sizer->AddSpacer(10);
		m_l = new ColorSlider(this, this, "亮度", false);
		sizer->Add(m_l);

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
	float h, s, l;
	rgb2hsl(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, h, s, l);

	int ih = (int)(h * 255 + 0.5f),
		is = (int)(s * 255 + 0.5f),
		il = (int)(l * 255 + 0.5f);
	m_h->SetColorValue(ih);
	m_s->SetColorValue(is);
	m_l->SetColorValue(il);
	m_h->SetColorRegion(s2::Color(0, is, il), s2::Color(255, is, il));
	m_s->SetColorRegion(s2::Color(ih, 0, il), s2::Color(ih, 255, il));
	m_l->SetColorRegion(s2::Color(ih, is, 0), s2::Color(ih, is, 255));

	m_color_bg->SetBackgroundColour(wxColour(col.r, col.g, col.b));
	m_color_bg->Refresh(true);

	if (m_lsn) {
		m_lsn->OnColorChanged(col);
	}
}

}
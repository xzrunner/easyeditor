#include "HSLColorSettingDlg.h"
#include "ColorSlider.h"
#include "convert_color.h"

namespace ee
{

HSLColorSettingDlg::HSLColorSettingDlg(wxWindow* parent, ColorMonitor* lsn, const Colorf& col, const wxPoint& pos)
	: wxDialog(parent, wxID_ANY, "HSL Color Setting", pos, wxSize(450, 300))
	, m_lsn(lsn)
	, m_h(NULL)
	, m_s(NULL)
	, m_l(NULL)
{
	InitLayout();
	SetColor(col);
}

Colorf HSLColorSettingDlg::GetColor() const
{
	return hsl2rgb(m_h->GetColorValue() / 255.0f, m_s->GetColorValue() / 255.0f, m_l->GetColorValue() / 255.0f);
}

void HSLColorSettingDlg::OnColorChanged()
{
	float h = m_h->GetColorValue() / 255.0f;
	float s = m_s->GetColorValue() / 255.0f;
	float l = m_l->GetColorValue() / 255.0f;
	m_h->SetColorRegion(Colorf(0, s, l), Colorf(1, s, l));
	m_s->SetColorRegion(Colorf(h, 0, l), Colorf(h, 1, l));
	m_l->SetColorRegion(Colorf(h, s, 0), Colorf(h, s, 1));

	Colorf rgb = hsl2rgb(m_h->GetColorValue() / 255.0f, m_s->GetColorValue() / 255.0f, m_l->GetColorValue() / 255.0f);
	m_color_bg->SetBackgroundColour(wxColour(rgb.r * 255 + 0.5f, rgb.g * 255 + 0.5f, rgb.b * 255 + 0.5f));
	m_color_bg->Refresh(true);

	if (m_lsn) {
		m_lsn->OnColorChanged(GetColor());
	}
}

void HSLColorSettingDlg::OnColorChanged(const Colorf& col)
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

void HSLColorSettingDlg::SetColor(const Colorf& col)
{
	Colorf hsl = rgb2hsl(col.r, col.g, col.b);

	Colorf test = hsl2rgb(hsl.r, hsl.g, hsl.b);

	m_h->SetColorValue(hsl.r * 255.0f + 0.5f);
	m_s->SetColorValue(hsl.g * 255.0f + 0.5f);
	m_l->SetColorValue(hsl.b * 255.0f + 0.5f);
	OnColorChanged(col);
}

}
#include "HSLColorSettingDlg.h"
#include "ColorSlider.h"

#include "common/color_convert.h"

namespace d2d
{

HSLColorSettingDlg::HSLColorSettingDlg(wxWindow* parent, IColorMonitor* lsn, const Colorf& col, const wxPoint& pos)
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
	Colori rgb = hsl2rgb(m_h->GetColorValue(), m_s->GetColorValue(), m_l->GetColorValue());

	// todo fix rgb2hsl
	if (rgb.r == 0 && rgb.g == 254 && rgb.b == 0) {
		rgb.g = 255;
	}

	return Colorf(rgb.r / 255.0f, rgb.g / 255.0f, rgb.b / 255.0f);
}

void HSLColorSettingDlg::OnColorChanged()
{
	float h = m_h->GetColorValue() / 255.0f;
	float s = m_s->GetColorValue() / 255.0f;
	float l = m_l->GetColorValue() / 255.0f;
	m_h->SetColorRegion(Colorf(0, s, l), Colorf(1, s, l));
	m_s->SetColorRegion(Colorf(h, 0, l), Colorf(h, 1, l));
	m_l->SetColorRegion(Colorf(h, s, 0), Colorf(h, s, 1));

	Colori rgb = hsl2rgb(m_h->GetColorValue(), 
		m_s->GetColorValue(), m_l->GetColorValue());
	m_color_bg->SetBackgroundColour(wxColour(rgb.r, rgb.g, rgb.b));
	m_color_bg->Refresh();

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
	SetSizer(top_sizer);
	top_sizer->Fit(this);
}

void HSLColorSettingDlg::SetColor(const Colorf& col)
{
	Colori hsl = rgb2hsl(col.r*255, col.g*255, col.b*255);
	m_h->SetColorValue(hsl.r);
	m_s->SetColorValue(hsl.g);
	m_l->SetColorValue(hsl.b);
	OnColorChanged(col);
}

}
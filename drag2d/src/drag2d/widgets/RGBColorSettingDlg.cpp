#include "RGBColorSettingDlg.h"
#include "ColorSlider.h"

namespace d2d
{

RGBColorSettingDlg::RGBColorSettingDlg(wxWindow* parent, const Colorf& col)
	: wxDialog(parent, wxID_ANY, "RGB Color Setting", wxDefaultPosition, wxSize(450, 300))
	, m_r(NULL)
	, m_g(NULL)
	, m_b(NULL)
{
	InitLayout();
	SetColor(col);
}

void RGBColorSettingDlg::OnColorChanged()
{
	float r = m_r->GetColorValue() / 255.0f;
	float g = m_g->GetColorValue() / 255.0f;
	float b = m_b->GetColorValue() / 255.0f;
	m_r->SetColorRegion(Colorf(0, g, b), Colorf(1, g, b));
	m_g->SetColorRegion(Colorf(r, 0, b), Colorf(r, 1, b));
	m_b->SetColorRegion(Colorf(r, g, 0), Colorf(r, g, 1));

	m_color_bg->SetBackgroundColour(wxColour(m_r->GetColorValue(), 
		m_g->GetColorValue(), m_b->GetColorValue()));
	m_color_bg->Refresh();
}

Colorf RGBColorSettingDlg::GetColor() const
{
	Colorf col;
	col.r = m_r->GetColorValue() / 255.0f;
	col.g = m_g->GetColorValue() / 255.0f;
	col.b = m_b->GetColorValue() / 255.0f;
	return col;
}

void RGBColorSettingDlg::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	// color
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "color");
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		m_color_bg = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(430, 100));
		sizer->Add(m_color_bg);
		top_sizer->Add(sizer);
	}
	// setting
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, "setting");
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

		m_r = new ColorSlider(this, this, "R", true);
		sizer->Add(m_r);
		sizer->AddSpacer(10);
		m_g = new ColorSlider(this, this, "G", true);
		sizer->Add(m_g);
		sizer->AddSpacer(10);
		m_b = new ColorSlider(this, this, "B", true);
		sizer->Add(m_b);

		top_sizer->Add(sizer);
	}	
	SetSizer(top_sizer);
	top_sizer->Fit(this);
}

void RGBColorSettingDlg::SetColor(const Colorf& col)
{
	m_r->SetColorValue(col.r * 255);
	m_g->SetColorValue(col.g * 255);
	m_b->SetColorValue(col.b * 255);
	OnColorChanged();
}

}
#include "RGBColorSettingDlg.h"
#include "ColorSlider.h"

namespace ee
{

RGBColorSettingDlg::RGBColorSettingDlg(wxWindow* parent, ColorMonitor* lsn, const s2::Color& col, const wxPoint& pos)
	: wxDialog(parent, wxID_ANY, "RGB Color Setting", pos, wxSize(450, 300))
	, m_lsn(lsn)
	, m_r(NULL)
	, m_g(NULL)
	, m_b(NULL)
{
	InitLayout();
	SetColor(col);
}

s2::Color RGBColorSettingDlg::GetColor() const
{
	s2::Color col;
	col.r = m_r->GetColorValue();
	col.g = m_g->GetColorValue();
	col.b = m_b->GetColorValue();
	return col;
}

void RGBColorSettingDlg::OnColorChanged()
{
	int r = m_r->GetColorValue();
	int g = m_g->GetColorValue();
	int b = m_b->GetColorValue();
	m_r->SetColorRegion(s2::Color(0, g, b), s2::Color(1, g, b));
	m_g->SetColorRegion(s2::Color(r, 0, b), s2::Color(r, 1, b));
	m_b->SetColorRegion(s2::Color(r, g, 0), s2::Color(r, g, 1));

	m_color_bg->SetBackgroundColour(wxColour(m_r->GetColorValue(), 
		m_g->GetColorValue(), m_b->GetColorValue()));
	m_color_bg->Refresh(true);

	if (m_lsn) {
		m_lsn->OnColorChanged(GetColor());
	}
}

void RGBColorSettingDlg::OnColorChanged(const s2::Color& col)
{
	OnColorChanged();
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
	// default
	{
		SetEscapeId(wxID_CANCEL);
		top_sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxCENTER);
	}
	SetSizer(top_sizer);
	top_sizer->Fit(this);
}

void RGBColorSettingDlg::SetColor(const s2::Color& col)
{
	m_r->SetColorValue(col.r);
	m_g->SetColorValue(col.g);
	m_b->SetColorValue(col.b);
	OnColorChanged(col);
}

}
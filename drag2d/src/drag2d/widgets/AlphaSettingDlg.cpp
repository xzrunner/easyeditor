#include "AlphaSettingDlg.h"
#include "ColorSlider.h"

#include <message/SetCanvasDirtySJ.h>

namespace d2d
{

AlphaSettingDlg::AlphaSettingDlg(wxWindow* parent, Colorf& color, const wxPoint& pos)
	: wxDialog(parent, wxID_ANY, "Alpha Setting", pos, wxSize(450, 300))
	, m_color(color)
	, m_alpha(NULL)
{
	InitLayout();
	SetColor(color);
}

Colorf AlphaSettingDlg::GetColor() const
{
	Colorf color;
	color.a = m_alpha->GetColorValue() / 255.0f;
	return color;
}

void AlphaSettingDlg::OnColorChanged()
{
	m_color.a = m_alpha->GetColorValue() / 255.0f;
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void AlphaSettingDlg::OnColorChanged(const Colorf& color)
{
	OnColorChanged();
}

void AlphaSettingDlg::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	m_alpha = new ColorSlider(this, this, "Alpha", true);
	top_sizer->Add(m_alpha);

	SetSizer(top_sizer);
	top_sizer->Fit(this);
}

void AlphaSettingDlg::SetColor(const Colorf& color)
{
	m_alpha->SetColorValue(color.a * 255);
	OnColorChanged(color);
}

}
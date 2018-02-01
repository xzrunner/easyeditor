#include "AlphaSettingDlg.h"
#include "ColorSlider.h"
#include "panel_msg.h"

#include <sprite2/Sprite.h>

namespace ee
{

AlphaSettingDlg::AlphaSettingDlg(wxWindow* parent, const SprPtr& spr, const wxPoint& pos)
	: wxDialog(parent, wxID_ANY, "Alpha Setting", pos, wxSize(450, 300))
	, m_spr(spr)
	, m_alpha(NULL)
{
	InitLayout();
	SetColor(spr->GetColorCommon().mul);
}

pt2::Color AlphaSettingDlg::GetColor() const
{
	pt2::Color color;
	color.a = m_alpha->GetColorValue();
	return color;
}

void AlphaSettingDlg::OnColorChanged()
{
	auto mul = m_spr->GetColorCommon().mul;
	mul.a = m_alpha->GetColorValue();
	m_spr->SetColorCommon(pt2::RenderColorCommon(
		mul, m_spr->GetColorCommon().add));
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void AlphaSettingDlg::OnColorChanged(const pt2::Color& color)
{
	OnColorChanged();
}

void AlphaSettingDlg::InitLayout()
{
	wxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	m_alpha = new ColorSlider(this, this, "Alpha", true);
	top_sizer->Add(m_alpha);

	SetEscapeId(wxID_CANCEL);
	top_sizer->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxCENTER);

	SetSizer(top_sizer);
	top_sizer->Fit(this);
}

void AlphaSettingDlg::SetColor(const pt2::Color& color)
{
	m_alpha->SetColorValue(color.a);
	OnColorChanged(color);
}

}
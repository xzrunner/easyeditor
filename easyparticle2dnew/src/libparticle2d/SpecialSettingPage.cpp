#include "SpecialSettingPage.h"
#include "CosSliderCtrl.h"
#include "StageData.h"

namespace eparticle2d
{

SpecialSettingPage::SpecialSettingPage(wxWindow* parent, StageData* stage_data)
	: SettingPageBase(parent)
	, m_stage_data(stage_data)
{
	InitLayout();
}

void SpecialSettingPage::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

 	CosSliderCtrl* s_cos = new CosSliderCtrl(this, "cos", m_stage_data);
 	sizer->Add(s_cos);
 	m_sliders.push_back(s_cos);

	SetSizer(sizer);
}

}
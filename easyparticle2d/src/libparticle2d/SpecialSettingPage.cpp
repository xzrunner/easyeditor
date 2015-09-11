#include "SpecialSettingPage.h"
#include "CosSliderCtrl.h"
#include "StageData.h"
#include "ps_config.h"
#include "item_string.h"

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
	InitHoriCosLayout(sizer);
	sizer->AddSpacer(20);
	InitCircularLayout(sizer);
	SetSizer(sizer);
}

void SpecialSettingPage::InitHoriCosLayout(wxSizer* sizer)
{
	CosSliderCtrl* s_cos = new CosSliderCtrl(this, "hori cos", m_stage_data);
	sizer->Add(s_cos);
	m_sliders.push_back(s_cos);
}

void SpecialSettingPage::InitCircularLayout(wxSizer* sizer)
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Circular"));
	wxBoxSizer* c_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	d2d::UICallback* cb = m_stage_data;

	d2d::SliderCtrlTwo* start_radius = new d2d::SliderCtrlTwo(this, "Start Radius", "Start Radius", cb, PS_START_RADIUS,
		d2d::SliderItem("center", ITEM_ATTR_CENTER, 0, 0, 1000), d2d::SliderItem("offset", ITEM_ATTR_OFFSET, 0, 0, 100));
	c_sizer->Add(start_radius);
	c_sizer->AddSpacer(10);
	m_sliders.push_back(start_radius);

	d2d::SliderCtrlTwo* end_radius = new d2d::SliderCtrlTwo(this, "End Radius", "End Radius", cb, PS_END_RADIUS,
		d2d::SliderItem("center", ITEM_ATTR_CENTER, 0, 0, 1000), d2d::SliderItem("offset", ITEM_ATTR_OFFSET, 0, 0, 100));
	c_sizer->Add(end_radius);
	c_sizer->AddSpacer(10);
	m_sliders.push_back(end_radius);

	d2d::SliderCtrlTwo* rotate = new d2d::SliderCtrlTwo(this, "Rotate", "Rotate", cb, PS_ROTATE_PER_SECOND,
		d2d::SliderItem("center", ITEM_ATTR_CENTER, 0, 0, 360), d2d::SliderItem("offset", ITEM_ATTR_OFFSET, 0, 0, 90));
	c_sizer->Add(rotate);
	c_sizer->AddSpacer(10);
	m_sliders.push_back(rotate);

	sizer->Add(c_sizer);
}

}
#include "CosSliderCtrl.h"
#include "item_string.h"
#include "ps_config.h"

namespace eparticle2d
{

CosSliderCtrl::CosSliderCtrl(wxPanel* parent, const char* title, ee::UICallback* cb)
	: wxPanel(parent, wxID_ANY)
	, m_cb(cb)
{
 	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, title);
 	wxBoxSizer* top_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

 	m_amplitude_ctrl = new ee::SliderCtrlTwo(this, "Õñ·ù", "amplitude", this, PS_COS_AMPLITUDE, 
		ee::SliderItem(ITEM_ATTR_CENTER, ITEM_ATTR_CENTER, 0, 0, 200), ee::SliderItem(ITEM_ATTR_OFFSET, ITEM_ATTR_OFFSET, 0, 0, 100));
	top_sizer->Add(m_amplitude_ctrl);

	top_sizer->AddSpacer(5);

	m_frequency_ctrl = new ee::SliderCtrlTwo(this, "ÆµÂÊ", "frequency", this, PS_COS_FREQUENCY, 
		ee::SliderItem(ITEM_ATTR_CENTER, ITEM_ATTR_CENTER, 0, 0, 500), ee::SliderItem(ITEM_ATTR_OFFSET, ITEM_ATTR_OFFSET, 0, 0, 500), 0.01f);
	top_sizer->Add(m_frequency_ctrl);

 	SetSizer(top_sizer);
}

void CosSliderCtrl::Update()
{
	m_amplitude_ctrl->Update();
	m_frequency_ctrl->Update();
}

void CosSliderCtrl::Load(const Json::Value& val, int version)
{
	m_amplitude_ctrl->Load(val["cos"], version);
	m_frequency_ctrl->Load(val["cos"], version);
}

void CosSliderCtrl::Store(Json::Value& val)
{
	m_amplitude_ctrl->Store(val["cos"]);
	m_frequency_ctrl->Store(val["cos"]);
}

void CosSliderCtrl::Load()
{
	m_amplitude_ctrl->Load();
	m_frequency_ctrl->Load();
}

void CosSliderCtrl::SetValue(int key, const Data& data)
{
	m_cb->SetValue(key, data);
}

void CosSliderCtrl::GetValue(int key, Data& data)
{
	m_cb->GetValue(key, data);	
}

}
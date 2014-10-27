#include "CosSliderCtrl.h"
#include "ParticleSystem.h"

namespace eparticle2d
{

CosSliderCtrl::CosSliderCtrl(wxPanel* parent, const char* title, ParticleSystem* ps)
	: wxPanel(parent, wxID_ANY)
	, m_ps(ps)
{
 	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, title);
 	wxBoxSizer* top_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

 	m_amplitude_ctrl = new SliderCtrlTwo(this, "Õñ·ù", "amplitude", this, PS_COS_AMPLITUDE, 
		SliderItem("center", "center", 0, 0, 200), SliderItem("offset", "offset", 0, 0, 100));
	top_sizer->Add(m_amplitude_ctrl);

	top_sizer->AddSpacer(5);

	m_frequency_ctrl = new SliderCtrlTwo(this, "ÆµÂÊ", "frequency", this, PS_COS_FREQUENCY, 
		SliderItem("center", "center", 0, 0, 100), SliderItem("offset", "offset", 0, 0, 100));
	top_sizer->Add(m_frequency_ctrl);

 	SetSizer(top_sizer);
}

void CosSliderCtrl::Update()
{
	m_amplitude_ctrl->Update();
	m_frequency_ctrl->Update();
}

void CosSliderCtrl::Load(const Json::Value& val)
{
	m_amplitude_ctrl->Load(val["cos"]);
	m_frequency_ctrl->Load(val["cos"]);
}

void CosSliderCtrl::Store(Json::Value& val)
{
	m_amplitude_ctrl->Store(val["cos"]);
	m_frequency_ctrl->Store(val["cos"]);
}

void CosSliderCtrl::OnSetKeyValue(int key, int val)
{
	m_ps->OnSetKeyValue(key, val);
}

void CosSliderCtrl::OnSetKeyValue(int key, int val0, int val1)
{
	m_ps->OnSetKeyValue(key, val0, val1);
}

}
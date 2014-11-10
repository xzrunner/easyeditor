#include "SettingPageBase.h"
#include "ISliderCtrl.h"

namespace eparticle2d
{

SettingPageBase::SettingPageBase(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
}

void SettingPageBase::Load(const Json::Value& val, int version)
{
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Load(val, version);
	}

	InitPSFromUI();
}

void SettingPageBase::Store(Json::Value& val) const
{
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Store(val);
	}
}

void SettingPageBase::ResetUIFromPS()
{
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Load();
	}
}

void SettingPageBase::InitPSFromUI()
{
	for (int i = 0, n = m_sliders.size(); i < n; ++i) {
		m_sliders[i]->Update();
	}
}

}
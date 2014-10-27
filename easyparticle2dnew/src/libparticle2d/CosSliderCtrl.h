#ifndef _EASYPARTICLE2D_COS_SLIDER_CTRL_H_
#define _EASYPARTICLE2D_COS_SLIDER_CTRL_H_

#include "slider_ctrl.h"
#include "UICallback.h"

namespace eparticle2d
{

class ParticleSystem;

class CosSliderCtrl : public ISliderCtrl, public wxPanel, public UICallback
{
public:
	CosSliderCtrl(wxPanel* parent, const char* title, ParticleSystem* ps);

	virtual void Update();
	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val);

	//
	// interface UICallback
	//
	virtual void OnSetKeyValue(int key, int val);
	virtual void OnSetKeyValue(int key, int val0, int val1);

private:
	ParticleSystem* m_ps;

	SliderCtrlTwo* m_amplitude_ctrl;
	SliderCtrlTwo* m_frequency_ctrl;

}; // CosSliderCtrl

}

#endif // _EASYPARTICLE2D_COS_SLIDER_CTRL_H_
#ifndef _EASYPARTICLE2D_COS_SLIDER_CTRL_H_
#define _EASYPARTICLE2D_COS_SLIDER_CTRL_H_

#include "ISliderCtrl.h"
#include "UICallback.h"

#include <drag2d.h>

namespace eparticle2d
{

class SliderCtrlTwo;

class CosSliderCtrl : public ISliderCtrl, public wxPanel, public UICallback
{
public:
	CosSliderCtrl(wxPanel* parent, const char* title, UICallback* cb);

	virtual void Update();
	virtual void Load(const Json::Value& val);
	virtual void Store(Json::Value& val);
	virtual void Load();

	//
	// interface UICallback
	//
	virtual void SetValue(int key, const Data& data);
	virtual void GetValue(int key, Data& data);

private:
	UICallback* m_cb;

	SliderCtrlTwo* m_amplitude_ctrl;
	SliderCtrlTwo* m_frequency_ctrl;

}; // CosSliderCtrl

}

#endif // _EASYPARTICLE2D_COS_SLIDER_CTRL_H_
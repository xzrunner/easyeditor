#ifndef _EASYPARTICLE2D_COS_SLIDER_CTRL_H_
#define _EASYPARTICLE2D_COS_SLIDER_CTRL_H_

#include <drag2d.h>

namespace eparticle2d
{

class d2d::SliderCtrlTwo;

class CosSliderCtrl : public d2d::ISliderCtrl, public wxPanel, public d2d::UICallback
{
public:
	CosSliderCtrl(wxPanel* parent, const char* title, d2d::UICallback* cb);

	virtual void Update();
	virtual void Load(const Json::Value& val, int version);
	virtual void Store(Json::Value& val);
	virtual void Load();

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const Data& data);
	virtual void GetValue(int key, Data& data);

private:
	d2d::UICallback* m_cb;

	d2d::SliderCtrlTwo* m_amplitude_ctrl;
	d2d::SliderCtrlTwo* m_frequency_ctrl;

}; // CosSliderCtrl

}

#endif // _EASYPARTICLE2D_COS_SLIDER_CTRL_H_
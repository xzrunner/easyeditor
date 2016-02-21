#ifndef _EASYPARTICLE2D_COS_SLIDER_CTRL_H_
#define _EASYPARTICLE2D_COS_SLIDER_CTRL_H_

#include <ee/SliderCtrl.h>
#include <ee/UICallback.h>

#include <wx/panel.h>

namespace ee { class SliderCtrlTwo; }

namespace eparticle2d
{

class CosSliderCtrl : public ee::SliderCtrl, public wxPanel, public ee::UICallback
{
public:
	CosSliderCtrl(wxPanel* parent, const char* title, ee::UICallback* cb);

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
	ee::UICallback* m_cb;

	ee::SliderCtrlTwo* m_amplitude_ctrl;
	ee::SliderCtrlTwo* m_frequency_ctrl;

}; // CosSliderCtrl

}

#endif // _EASYPARTICLE2D_COS_SLIDER_CTRL_H_
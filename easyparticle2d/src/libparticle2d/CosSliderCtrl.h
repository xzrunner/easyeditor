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

	virtual void Update() override;
	virtual void Load(const Json::Value& val, int version) override;
	virtual void Store(Json::Value& val) override;
	virtual void Load() override;

	//
	// UICallback interface
	//
	virtual void SetValue(int key, const Data& data) override;
	virtual void GetValue(int key, Data& data) override;

private:
	ee::UICallback* m_cb;

	ee::SliderCtrlTwo* m_amplitude_ctrl;
	ee::SliderCtrlTwo* m_frequency_ctrl;

}; // CosSliderCtrl

}

#endif // _EASYPARTICLE2D_COS_SLIDER_CTRL_H_
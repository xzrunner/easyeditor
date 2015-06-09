#ifndef _EASYSHADOW_TOOLBAR_PANEL_H_
#define _EASYSHADOW_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eshadow
{

class StagePanel;

class ToolbarPanel : public d2d::ToolbarPanel, public d2d::UICallback
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage);

	//
	// interface d2d::UICallback
	//
	virtual void SetValue(int key, const d2d::UICallback::Data& data);
	virtual void GetValue(int key, d2d::UICallback::Data& data);

protected:
	virtual wxSizer* initLayout();

private:
	void OnSetInnerColor(wxCommandEvent& event);
	void OnSetOuterColor(wxCommandEvent& event);

private:
	enum SLIDER_ID
	{
		S_RADIUS = 0
	};

private:
	StagePanel* m_stage;

	d2d::SliderCtrlOne* m_slider_radius;

}; // ToolbarPanel

}

#endif // _EASYSHADOW_TOOLBAR_PANEL_H_
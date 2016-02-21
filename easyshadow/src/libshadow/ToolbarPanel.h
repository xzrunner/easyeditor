#ifndef _EASYSHADOW_TOOLBAR_PANEL_H_
#define _EASYSHADOW_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>
#include <ee/UICallback.h>

namespace ee { class SliderCtrlOne; }

namespace eshadow
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel, public ee::UICallback
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage);

	//
	// interface ee::UICallback
	//
	virtual void SetValue(int key, const ee::UICallback::Data& data);
	virtual void GetValue(int key, ee::UICallback::Data& data);

protected:
	virtual wxSizer* InitLayout();

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

	ee::SliderCtrlOne* m_slider_radius;

}; // ToolbarPanel

}

#endif // _EASYSHADOW_TOOLBAR_PANEL_H_
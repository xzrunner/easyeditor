#ifndef _EASYSHADOW_TOOLBAR_PANEL_H_
#define _EASYSHADOW_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eshadow
{

class StagePanel;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage);

protected:
	virtual wxSizer* initLayout();

private:
	void OnSetInnerColor(wxCommandEvent& event);
	void OnSetOuterColor(wxCommandEvent& event);

	void OnChangeRadius(wxSpinEvent& event);

private:
	StagePanel* m_stage;

}; // ToolbarPanel

}

#endif // _EASYSHADOW_TOOLBAR_PANEL_H_
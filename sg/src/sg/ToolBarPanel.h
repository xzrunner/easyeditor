#ifndef _SG_TOOLBAR_PANEL_H_
#define _SG_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace sg
{

class StagePanel;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library, StagePanel* stage);

protected:
	virtual wxSizer* initLayout();

private:
	void initOptSetting(wxSizer* sizer);
	void initSizeSetting(wxBoxSizer* topSizer);

	void onParamsChanged(wxSpinEvent& event);

	void onLevelChanged(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	wxSpinCtrl *m_row, *m_col;
	wxSpinCtrl* m_edge;

}; // ToolbarPanel

}

#endif // _SG_TOOLBAR_PANEL_H_
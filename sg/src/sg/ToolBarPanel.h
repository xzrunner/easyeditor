#ifndef _SG_TOOLBAR_PANEL_H_
#define _SG_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace sg
{

class StagePanel;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library, 
		d2d::PropertySettingPanel* property, StagePanel* stage);

protected:
	virtual wxSizer* initLayout();

private:
	void onParamsChanged(wxSpinEvent& event);

private:
	wxSpinCtrl *m_row, *m_col;
	wxSpinCtrl* m_edge;

}; // ToolbarPanel

}

#endif // _SG_TOOLBAR_PANEL_H_
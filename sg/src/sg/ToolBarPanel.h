#ifndef _SG_TOOLBAR_PANEL_H_
#define _SG_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace ee { class LibraryPanel; class PropertySettingPanel; }

namespace sg
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, ee::LibraryPanel* library, 
		ee::PropertySettingPanel* property, StagePanel* stage);

protected:
	virtual wxSizer* InitLayout();

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
#ifndef _EASYSHAPE_TOOLBAR_PANEL_H_
#define _EASYSHAPE_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace ee { class PropertySettingPanel; }

namespace eshape
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, ee::PropertySettingPanel* property,
		StagePanel* stage);

	void SelectSuitableEditOP();

protected:
	virtual wxSizer* InitLayout();

private:
	void OnClearShapes(wxCommandEvent& event);
	void OnCreateBounding(wxCommandEvent& event);

private:
	StagePanel* m_stage_panel;

}; // ToolbarPanel

}

#endif // _EASYSHAPE_TOOLBAR_PANEL_H_
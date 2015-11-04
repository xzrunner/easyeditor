#ifndef _LIBSHAPE_TOOLBAR_PANEL_H_
#define _LIBSHAPE_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace libshape
{

class StagePanel;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, d2d::PropertySettingPanel* property,
		StagePanel* stage);

	void SelectSuitableEditOP();

protected:
	virtual wxSizer* initLayout();

private:
	void OnClearShapes(wxCommandEvent& event);
	void OnCreateBounding(wxCommandEvent& event);

private:
	StagePanel* m_stage_panel;

}; // ToolbarPanel

}

#endif // _LIBSHAPE_TOOLBAR_PANEL_H_
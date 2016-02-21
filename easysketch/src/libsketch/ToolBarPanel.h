#ifndef _EASYSKETCH_TOOLBAR_PANEL_H_
#define _EASYSKETCH_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace esketch
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage);

protected:
	virtual wxSizer* InitLayout();

}; // ToolbarPanel

}

#endif // _EASYSKETCH_TOOLBAR_PANEL_H_

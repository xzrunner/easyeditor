#ifndef _EASYBONE_TOOLBAR_PANEL_H_
#define _EASYBONE_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace ebone
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

#endif // _EASYBONE_TOOLBAR_PANEL_H_

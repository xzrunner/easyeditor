#ifndef _EASYP3DINV_TOOLBAR_PANEL_H_
#define _EASYP3DINV_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace ep3dinv
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

#endif // _EASYP3DINV_TOOLBAR_PANEL_H_
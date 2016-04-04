#ifndef _EASYNAVMESH_TOOLBAR_PANEL_H_
#define _EASYNAVMESH_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace enav
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

#endif // _EASYNAVMESH_TOOLBAR_PANEL_H_
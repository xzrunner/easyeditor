#ifndef _EASYTEMPLATE_TOOLBAR_PANEL_H_
#define _EASYTEMPLATE_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace etemplate
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

#endif // _EASYTEMPLATE_TOOLBAR_PANEL_H_
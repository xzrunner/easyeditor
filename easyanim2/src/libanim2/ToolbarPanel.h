#ifndef _LIBANIM2_TOOLBAR_PANEL_H_
#define _LIBANIM2_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace libanim2
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage);

protected:
	virtual wxSizer* InitLayout() override;

}; // ToolbarPanel

}

#endif // _LIBANIM2_TOOLBAR_PANEL_H_
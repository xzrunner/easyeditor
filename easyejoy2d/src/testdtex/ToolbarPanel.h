#ifndef _TEST_DTEX_TOOLBAR_PANEL_H_
#define _TEST_DTEX_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace tdtex
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

#endif // _TEST_DTEX_TOOLBAR_PANEL_H_
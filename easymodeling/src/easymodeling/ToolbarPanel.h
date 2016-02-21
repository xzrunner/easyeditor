#ifndef _EASYMODELING_TOOLBAR_PANEL_H_
#define _EASYMODELING_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace emodeling
{

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent);

protected:
	virtual wxSizer* InitLayout();

}; // ToolbarPanel

}

#endif // _EASYMODELING_TOOLBAR_PANEL_H_

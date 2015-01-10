#ifndef _EASYTEMPLATE_TOOLBAR_PANEL_H_
#define _EASYTEMPLATE_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace etemplate
{

class StagePanel;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage);

protected:
	virtual wxSizer* initLayout();

}; // ToolbarPanel

}

#endif // _EASYTEMPLATE_TOOLBAR_PANEL_H_
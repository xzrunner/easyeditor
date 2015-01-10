#ifndef _EASYTERRAIN2D_TOOLBAR_PANEL_H_
#define _EASYTERRAIN2D_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eterrain2d
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

#endif // _EASYTERRAIN2D_TOOLBAR_PANEL_H_
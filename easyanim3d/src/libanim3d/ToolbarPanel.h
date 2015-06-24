#ifndef _EASYANIM3D_TOOLBAR_PANEL_H_
#define _EASYANIM3D_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eanim3d
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

#endif // _EASYANIM3D_TOOLBAR_PANEL_H_
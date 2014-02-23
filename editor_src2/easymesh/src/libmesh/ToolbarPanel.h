#ifndef _EASYMESH_TOOLBAR_PANEL_H_
#define _EASYMESH_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace emesh
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

#endif // _EASYMESH_TOOLBAR_PANEL_H_
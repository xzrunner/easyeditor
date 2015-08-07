#ifndef _DRAG2D_TOOLBAR_PANEL_H_
#define _DRAG2D_TOOLBAR_PANEL_H_

#include "component/AbstractEditCMPT.h"

namespace d2d
{

class AbstractEditCMPT;

class ToolbarPanel : public AbstractEditCMPT
{
public:
	ToolbarPanel(wxWindow* parent, EditPanelImpl* stage, bool vertical = true);

}; // ToolbarPanel

}

#endif // _DRAG2D_TOOLBAR_PANEL_H_
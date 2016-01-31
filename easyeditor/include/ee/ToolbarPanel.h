#ifndef _EASYEDITOR_TOOLBAR_PANEL_H_
#define _EASYEDITOR_TOOLBAR_PANEL_H_

#include "EditCMPT.h"

namespace ee
{

class EditCMPT;

class ToolbarPanel : public EditCMPT
{
public:
	ToolbarPanel(wxWindow* parent, EditPanelImpl* stage, bool vertical = true);

}; // ToolbarPanel

}

#endif // _EASYEDITOR_TOOLBAR_PANEL_H_
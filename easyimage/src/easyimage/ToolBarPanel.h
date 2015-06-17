#ifndef _EASYIMAGE_TOOLBAR_PANEL_H_
#define _EASYIMAGE_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eimage
{

class StagePanel;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library, StagePanel* stage);

protected:
	virtual wxSizer* initLayout();

}; // ToolbarPanel

}

#endif // _EASYIMAGE_TOOLBAR_PANEL_H_

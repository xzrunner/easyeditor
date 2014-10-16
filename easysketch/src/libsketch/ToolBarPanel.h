#ifndef _LIBSKETCH_TOOLBAR_PANEL_H_
#define _LIBSKETCH_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace libsketch
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

#endif // _LIBSKETCH_TOOLBAR_PANEL_H_

#ifndef _ESHADER_TOOLBAR_PANEL_H_
#define _ESHADER_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eshader
{

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, d2d::EditPanel* stage);

protected:
	virtual wxSizer* initLayout();

}; // ToolbarPanel

}

#endif // _ESHADER_TOOLBAR_PANEL_H_
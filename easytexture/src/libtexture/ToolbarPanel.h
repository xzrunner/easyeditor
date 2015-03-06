#ifndef _EASYTEXTURE_TOOLBAR_PANEL_H_
#define _EASYTEXTURE_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace etexture
{

class StagePanel;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage,
		d2d::PropertySettingPanel* property);

protected:
	virtual wxSizer* initLayout();

}; // ToolbarPanel

}

#endif // _EASYTEXTURE_TOOLBAR_PANEL_H_

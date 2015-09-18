#ifndef _EASYICON_TOOLBAR_PANEL_H_
#define _EASYICON_TOOLBAR_PANEL_H_

#include <drag2d.h>

#include "IconType.h"

namespace eicon
{

class StagePanel;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage);

	void ChangeIconType(IconType type);

protected:
	virtual wxSizer* initLayout();

}; // ToolbarPanel

}

#endif // _EASYICON_TOOLBAR_PANEL_H_
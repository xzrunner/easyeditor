#ifndef _EASYICON_TOOLBAR_PANEL_H_
#define _EASYICON_TOOLBAR_PANEL_H_

#include "IconType.h"

#include <ee/ToolbarPanel.h>

namespace eicon
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage);

	void ChangeIconType(IconType type);

protected:
	virtual wxSizer* InitLayout();

}; // ToolbarPanel

}

#endif // _EASYICON_TOOLBAR_PANEL_H_
#ifndef _EASYCOMPLEX3D_TOOLBAR_PANEL_H_
#define _EASYCOMPLEX3D_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace ecomplex3d
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage);

protected:
	virtual wxSizer* InitLayout() override;

}; // ToolbarPanel

}

#endif // _EASYCOMPLEX3D_TOOLBAR_PANEL_H_

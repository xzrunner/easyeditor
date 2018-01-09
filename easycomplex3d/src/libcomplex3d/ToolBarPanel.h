#ifndef _EASYCOMPLEX3D_TOOLBAR_PANEL_H_
#define _EASYCOMPLEX3D_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace enode3d { class StagePanel; }

namespace ecomplex3d
{

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, enode3d::StagePanel* stage);

protected:
	virtual wxSizer* InitLayout() override;

}; // ToolbarPanel

}

#endif // _EASYCOMPLEX3D_TOOLBAR_PANEL_H_

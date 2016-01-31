#ifndef _EASYANIM3D_TOOLBAR_PANEL_H_
#define _EASYANIM3D_TOOLBAR_PANEL_H_



namespace eanim3d
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage);

protected:
	virtual wxSizer* InitLayout();

}; // ToolbarPanel

}

#endif // _EASYANIM3D_TOOLBAR_PANEL_H_
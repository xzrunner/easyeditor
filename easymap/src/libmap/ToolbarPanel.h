#ifndef _EASYMAP_TOOLBAR_PANEL_H_
#define _EASYMAP_TOOLBAR_PANEL_H_



namespace emap
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

#endif // _EASYMAP_TOOLBAR_PANEL_H_

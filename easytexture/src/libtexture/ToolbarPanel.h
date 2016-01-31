#ifndef _EASYTEXTURE_TOOLBAR_PANEL_H_
#define _EASYTEXTURE_TOOLBAR_PANEL_H_



namespace etexture
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

#endif // _EASYTEXTURE_TOOLBAR_PANEL_H_

#ifndef _LIBSKETCH_TOOLBAR_PANEL_H_
#define _LIBSKETCH_TOOLBAR_PANEL_H_



namespace libsketch
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

#endif // _LIBSKETCH_TOOLBAR_PANEL_H_

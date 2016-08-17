#ifndef _EASYMESH_TOOLBAR_PANEL_H_
#define _EASYMESH_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace emesh
{

class StagePanel;
class Sprite;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage, bool full,
		Sprite* spr);

protected:
	virtual wxSizer* InitLayout();

	void OnSetSpeed(wxCommandEvent& event);

private:
	Sprite* m_spr;

	wxTextCtrl *m_ctrl_xspeed, *m_ctrl_yspeed;

}; // ToolbarPanel

}

#endif // _EASYMESH_TOOLBAR_PANEL_H_
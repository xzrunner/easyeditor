#ifndef _EASYMESH_TOOLBAR_PANEL_H_
#define _EASYMESH_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace emesh
{

class StagePanel;
class Sprite;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage, bool full,
		Sprite* sprite);

protected:
	virtual wxSizer* initLayout();

	void OnSetSpeed(wxCommandEvent& event);

private:
	Sprite* m_sprite;

	wxTextCtrl *m_ctrl_xspeed, *m_ctrl_yspeed;

}; // ToolbarPanel

}

#endif // _EASYMESH_TOOLBAR_PANEL_H_
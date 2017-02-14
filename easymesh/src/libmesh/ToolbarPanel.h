#ifndef _EASYMESH_TOOLBAR_PANEL_H_
#define _EASYMESH_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace emesh
{

class StagePanel;
class Sprite;
class CreateMeshCMPT;
class EditMeshCMPT;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage, bool full,
		Sprite* spr);

	void SetEditType(int pm_mesh_type);

protected:
	virtual wxSizer* InitLayout();

	void OnSetSpeed(wxCommandEvent& event);

private:
	void OnChangeType(wxCommandEvent& event);

private:
	StagePanel* m_stage;

	Sprite* m_spr;

	CreateMeshCMPT* m_create_cmpt;
	EditMeshCMPT* m_edit_cmpt;

	wxRadioBox* m_type_choice;

	wxTextCtrl *m_ctrl_xspeed, *m_ctrl_yspeed;

}; // ToolbarPanel

}

#endif // _EASYMESH_TOOLBAR_PANEL_H_
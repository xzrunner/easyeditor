#ifndef _EASYTERRAIN2D_MOVE_TEXCOORDS_CMPT_H_
#define _EASYTERRAIN2D_MOVE_TEXCOORDS_CMPT_H_



namespace eterrain2d
{

class StagePanel;
class OceanMesh;

class MoveTexcoordsCMPT : public ee::EditCMPT
{
public:
	MoveTexcoordsCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

	void SetControlersValue(const OceanMesh* ocean);

protected:
	virtual wxSizer* InitLayout();

private:
	void OnOpenMoveChanged(wxCommandEvent& event);
	void OnChangeUVSpeed(wxSpinEvent& event);

private:
	StagePanel* m_stage_panel;

	wxCheckBox* m_switch;

	wxSpinCtrl *m_speed_x, *m_speed_y;

}; // MoveTexcoordsCMPT

}

#endif // _EASYTERRAIN2D_MOVE_TEXCOORDS_CMPT_H_
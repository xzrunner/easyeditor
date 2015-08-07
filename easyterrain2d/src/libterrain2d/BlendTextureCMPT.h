#ifndef _EASYTERRAIN2D_BLEND_TEXTURE_CMPT_H_
#define _EASYTERRAIN2D_BLEND_TEXTURE_CMPT_H_

#include <drag2d.h>

namespace eterrain2d
{

class StagePanel;
class OceanMesh;

class BlendTextureCMPT : public d2d::AbstractEditCMPT
{
public:
	BlendTextureCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

	void SetControlersValue(const OceanMesh* ocean);

protected:
	virtual wxSizer* initLayout();

private:
	void OnOpenBlendChanged(wxCommandEvent& event);
	void OnChangeSpeed(wxSpinEvent& event);

private:
	StagePanel* m_stage_panel;

	wxCheckBox* m_switch;

	wxSpinCtrl* m_speed;

}; // BlendTextureCMPT

}

#endif // _EASYTERRAIN2D_BLEND_TEXTURE_CMPT_H_
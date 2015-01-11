#ifndef _EASYTERRAIN2D_WAVE_VERTICES_CMPT_H_
#define _EASYTERRAIN2D_WAVE_VERTICES_CMPT_H_

#include <drag2d.h>

namespace eterrain2d
{

class StagePanel;

class WaveVerticesCMPT : public d2d::AbstractEditCMPT
{
public:
	WaveVerticesCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage);

protected:
	virtual wxSizer* initLayout();

private:
	void OnOpenWaveChanged(wxCommandEvent& event);
	void OnReflushPress(wxCommandEvent& event);
	void OnChangeGridSize(wxSpinEvent& event);
	void OnChangeWaveParams(wxSpinEvent& event);
	void OnChangeDisplayTriangles(wxCommandEvent& event);

private:
	wxSpinCtrl *m_row_spin, *m_col_spin;

	wxSpinCtrl *m_wave_speed, *m_wave_height;

}; // WaveVerticesCMPT

}

#endif // _EASYTERRAIN2D_WAVE_VERTICES_CMPT_H_
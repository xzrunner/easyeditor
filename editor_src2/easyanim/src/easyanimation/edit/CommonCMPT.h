#ifndef EANIM_COMMON_CMPT_H
#define EANIM_COMMON_CMPT_H

#include <drag2d.h>

namespace eanim
{

class StagePanel;

class CommonCMPT : public d2d::AbstractEditCMPT
{
public:
	CommonCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage, bool vertical);

protected:
	virtual wxSizer* initLayout();

private:
	wxSizer* initEditPanel();

	void onLoadFromFolder(wxCommandEvent& event);
	void onLoadFromList(wxCommandEvent& event);
	void onFillingFrames(wxCommandEvent& event);
	void onChangeAnim(wxCommandEvent& event);

private:
	bool m_vertical;

	wxSpinCtrl* m_filling;

}; // CommonCMPT

}

#endif // EANIM_COMMON_CMPT_H
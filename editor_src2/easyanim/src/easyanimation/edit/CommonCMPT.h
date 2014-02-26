#ifndef EANIM_COMMON_CMPT_H
#define EANIM_COMMON_CMPT_H

#include <drag2d.h>

namespace eanim
{

class StagePanel;
class StageSettings;

class CommonCMPT : public d2d::AbstractEditCMPT
{
public:
	CommonCMPT(wxWindow* parent, const wxString& name,
		StagePanel* stage, bool vertical);

protected:
	virtual wxSizer* initLayout();

private:
	wxSizer* initEditPanel();

	void initLoadPanel(wxSizer* sizer);
	void initFillingPanel(wxSizer* sizer);
	void initSettingsPanel(wxSizer* sizer);

	void onLoadFromFolder(wxCommandEvent& event);
	void onLoadFromList(wxCommandEvent& event);

	void onFillingFrames(wxCommandEvent& event);

	void onChangeAnim(wxCommandEvent& event);

	void onSetCross(wxCommandEvent& event);

private:
	bool m_vertical;

	StageSettings& m_settings;

	wxSpinCtrl* m_filling;

}; // CommonCMPT

}

#endif // EANIM_COMMON_CMPT_H
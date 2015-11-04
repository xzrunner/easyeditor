#ifndef EANIM_COMMON_CMPT_H
#define EANIM_COMMON_CMPT_H

#include <drag2d.h>

namespace eanim
{

class StagePanel;
class Controller;

class CommonCMPT : public d2d::AbstractEditCMPT
{
public:
	CommonCMPT(wxWindow* parent, const std::string& name,
		StagePanel* stage, d2d::PropertySettingPanel* property,
		bool vertical, Controller* ctrl);

protected:
	virtual wxSizer* initLayout();

private:
	wxSizer* initEditPanel();

	wxSizer* initLoadPanel();
	wxSizer* initFillingPanel();
	wxSizer* initSettingsPanel();

	void onLoadFromFolder(wxCommandEvent& event);
	void onLoadFromList(wxCommandEvent& event);

	void onFillingFrames(wxCommandEvent& event);

	void onChangeAnim(wxCommandEvent& event);

	void onAddCross(wxCommandEvent& event);
	void onDelCross(wxCommandEvent& event);

	void clear();

private:
	bool m_vertical;

	wxSpinCtrl* m_filling;

	Controller* m_ctrl;

}; // CommonCMPT

}

#endif // EANIM_COMMON_CMPT_H
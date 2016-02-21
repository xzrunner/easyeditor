#ifndef _EASYANIM_COMMON_CMPT_H_
#define _EASYANIM_COMMON_CMPT_H_

#include <ee/EditCMPT.h>

namespace eanim
{

class LayersMgr;
class StagePanel;

class CommonCMPT : public ee::EditCMPT
{
public:
	CommonCMPT(wxWindow* parent, const std::string& name, bool vertical);

protected:
	virtual wxSizer* InitLayout();

private:
	wxSizer* InitEditPanel();

	wxSizer* InitLoadPanel();
	wxSizer* InitFillingPanel();
	wxSizer* InitSettingsPanel();

	void OnLoadFromFolder(wxCommandEvent& event);
	void OnLoadFromList(wxCommandEvent& event);

	void OnFillingFrames(wxCommandEvent& event);

	void OnChangeAnim(wxCommandEvent& event);

	void OnAddCross(wxCommandEvent& event);
	void OnDelCross(wxCommandEvent& event);

private:
	bool m_vertical;

	wxSpinCtrl* m_filling;

}; // CommonCMPT

}

#endif // _EASYANIM_COMMON_CMPT_H_
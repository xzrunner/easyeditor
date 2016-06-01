#ifndef _EASYANIM_NORMAL_CMPT_H_
#define _EASYANIM_NORMAL_CMPT_H_

#include <ee/EditCMPT.h>

namespace eanim
{

class LayersMgr;
class StagePanel;

class NormalCMPT : public ee::EditCMPT
{
public:
	NormalCMPT(wxWindow* parent, const std::string& name, bool vertical);

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

}; // NormalCMPT

}

#endif // _EASYANIM_NORMAL_CMPT_H_
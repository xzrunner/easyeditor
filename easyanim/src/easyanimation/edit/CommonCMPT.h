#ifndef EANIM_COMMON_CMPT_H
#define EANIM_COMMON_CMPT_H

#include <drag2d.h>

namespace eanim
{

class LayersMgr;
class StagePanel;

class CommonCMPT : public d2d::AbstractEditCMPT
{
public:
	CommonCMPT(wxWindow* parent, const std::string& name,
		LayersMgr* layers, StagePanel* stage, 
		d2d::PropertySettingPanel* property,
		bool vertical);

protected:
	virtual wxSizer* initLayout();

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
	LayersMgr* m_layers;

	bool m_vertical;

	wxSpinCtrl* m_filling;

}; // CommonCMPT

}

#endif // EANIM_COMMON_CMPT_H
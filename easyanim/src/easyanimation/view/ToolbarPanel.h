#ifndef _EASYANIM_TOOLBAR_PANEL_H_
#define _EASYANIM_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eanim
{

class Controller;
class StagePanel;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage, 
		d2d::PropertySettingPanel* property, 
		bool vertical, Controller* ctrl);

	void AddAnimChoice(const std::vector<std::string>& choices);

	void Clear();

protected:
	virtual wxSizer* initLayout();

private:
	wxSizer* InitTemplateLayout();
	void FillTempSizer();

	void OnChangeAnim(wxCommandEvent& event);
	void OnSetTemplateDir(wxCommandEvent& event);

private:
	Controller* m_ctrl;

	wxSizer* m_temp_sizer;
	wxTextCtrl* m_temp_dir;
	wxChoice* m_anim_choice;

}; // ToolbarPanel

}

#endif // _EASYANIM_TOOLBAR_PANEL_H_
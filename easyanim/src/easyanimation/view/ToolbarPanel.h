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

	void ChangeTemplateMode(bool single);

	void Clear();

protected:
	virtual wxSizer* initLayout();

private:
	wxSizer* InitTemplateLayout();

	void OnChangeAnim(wxCommandEvent& event);
	void OnSetTemplateDir(wxCommandEvent& event);

private:
	Controller* m_ctrl;

	wxSizer* m_tl_sizer;

	wxSizer* m_tl_single_sizer;
	wxTextCtrl* m_tl_dir_text;

	wxSizer* m_tl_multi_sizer;
	wxChoice* m_tl_anim_choice;

}; // ToolbarPanel

}

#endif // _EASYANIM_TOOLBAR_PANEL_H_
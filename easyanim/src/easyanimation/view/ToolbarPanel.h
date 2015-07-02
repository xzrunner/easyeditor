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
		d2d::ViewPanelMgr* view_panel_mgr,
		bool vertical, Controller* ctrl);

	void AddAnimChoice(const std::vector<std::string>& choices);

protected:
	virtual wxSizer* initLayout();

private:
	wxSizer* InitTemplateLayout();

	void OnChangeAnim(wxCommandEvent& event);
	void OnOpenTemplate(wxCommandEvent& event);

private:
	Controller* m_ctrl;

	wxSizer* m_temp_sizer;
	wxCheckBox* m_temp_open;
	wxChoice* m_anim_choice;

}; // ToolbarPanel

}

#endif // _EASYANIM_TOOLBAR_PANEL_H_
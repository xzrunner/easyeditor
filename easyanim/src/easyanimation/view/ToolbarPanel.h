#ifndef _EASYANIM_TOOLBAR_PANEL_H_
#define _EASYANIM_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace eanim
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, bool vertical);

	void AddAnimChoice(const std::vector<std::string>& choices);

	void ChangeTemplateMode(bool single);

	void Clear();

protected:
	virtual wxSizer* InitLayout();

private:
	wxSizer* InitTemplateLayout();
	wxSizer* InitCommonLayout();

	void OnChangeAnim(wxCommandEvent& event);
	void OnSetTemplateDir(wxCommandEvent& event);

	void OnOpPass(wxCommandEvent& event);

private:
	wxSizer* m_tl_sizer;

	wxSizer* m_tl_single_sizer;
	wxTextCtrl* m_tl_dir_text;

	wxSizer* m_tl_multi_sizer;
	wxChoice* m_tl_anim_choice;

}; // ToolbarPanel

}

#endif // _EASYANIM_TOOLBAR_PANEL_H_
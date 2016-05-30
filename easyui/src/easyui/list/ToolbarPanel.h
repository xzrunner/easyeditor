#ifndef _EASYUI_LIST_TOOLBAR_PANEL_H_
#define _EASYUI_LIST_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace eui
{
namespace list
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage_panel);

	void EnableHori(bool enable);
	void EnableVert(bool enable);

	void EnableReverseOrder(bool enable);

	void SetSizeText(int width, int height);

protected:
	virtual wxSizer* InitLayout();

private:
	void OnItemFilling(wxCommandEvent& event);

	void OnChangeSize(wxCommandEvent& event);

	void OnChangeHori(wxCommandEvent& event);
	void OnChangeVert(wxCommandEvent& event);

	void OnReverseOrder(wxCommandEvent& event);

private:
	StagePanel* m_stage_panel;

	wxTextCtrl* m_width_text;
	wxTextCtrl* m_height_text;

	wxCheckBox* m_hori_check;
	wxCheckBox* m_vert_check;

	wxCheckBox* m_reverse_check;

}; // ToolbarPanel

}
}

#endif // _EASYUI_LIST_TOOLBAR_PANEL_H_
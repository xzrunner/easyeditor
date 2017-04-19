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

	void EnableReverseOrder(bool enable);
	void EnableColumnOrder(bool enable);

	void SetSizeText(int width, int height);

protected:
	virtual wxSizer* InitLayout();

private:
	void OnItemFilling(wxCommandEvent& event);

	void OnChangeSize(wxCommandEvent& event);

	void OnChangeCheck(wxCommandEvent& event);

	void OnReverseOrder(wxCommandEvent& event);
	void OnColumnOrder(wxCommandEvent& event);

private:
	StagePanel* m_stage_panel;

	wxTextCtrl* m_width_text;
	wxTextCtrl* m_height_text;

	wxChoice* m_check_dir;

	wxCheckBox* m_reverse_check;
	wxCheckBox* m_column_check; // column major order

}; // ToolbarPanel

}
}

#endif // _EASYUI_LIST_TOOLBAR_PANEL_H_
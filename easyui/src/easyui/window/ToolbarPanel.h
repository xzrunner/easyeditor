#ifndef _EASYUI_WINDOW_TOOLBAR_PANEL_H_
#define _EASYUI_WINDOW_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eui
{
namespace window
{

class StagePanel;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage_panel);

	

protected:
	virtual wxSizer* initLayout();

private:
	void OnChangeSize(wxCommandEvent& event);

private:
	StagePanel* m_stage_panel;

	wxTextCtrl* m_width_text;
	wxTextCtrl* m_height_text;

}; // ToolbarPanel


}
}

#endif // _EASYUI_WINDOW_TOOLBAR_PANEL_H_
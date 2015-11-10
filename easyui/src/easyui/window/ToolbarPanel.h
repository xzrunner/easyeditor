#ifndef _EASYUI_WINDOW_TOOLBAR_PANEL_H_
#define _EASYUI_WINDOW_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eui
{
namespace window
{

class StagePanel;

class ToolbarPanel : public d2d::ToolbarPanel, public d2d::Observer
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage_panel);
	virtual ~ToolbarPanel();

	//
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	std::string GetWindowName() const;
	void SetWindowName(const std::string& name);

protected:
	virtual wxSizer* initLayout();

private:
	void OnChangeSize(wxCommandEvent& event);

private:
	StagePanel* m_stage_panel;

	wxTextCtrl* m_name_text;

	wxTextCtrl* m_width_text;
	wxTextCtrl* m_height_text;

}; // ToolbarPanel


}
}

#endif // _EASYUI_WINDOW_TOOLBAR_PANEL_H_
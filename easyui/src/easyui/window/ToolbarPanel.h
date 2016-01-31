#ifndef _EASYUI_WINDOW_TOOLBAR_PANEL_H_
#define _EASYUI_WINDOW_TOOLBAR_PANEL_H_



namespace eui
{
namespace window
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel, public ee::Observer
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage_panel);

	std::string GetWindowName() const;
	void SetWindowName(const std::string& name);

protected:
	virtual wxSizer* InitLayout();

	//
	//	interface Observer
	//
	virtual void OnNotify(int sj_id, void* ud);

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
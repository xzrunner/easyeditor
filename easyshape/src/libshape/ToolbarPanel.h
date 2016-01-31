#ifndef _LIBSHAPE_TOOLBAR_PANEL_H_
#define _LIBSHAPE_TOOLBAR_PANEL_H_



namespace libshape
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, ee::PropertySettingPanel* property,
		StagePanel* stage);

	void SelectSuitableEditOP();

protected:
	virtual wxSizer* InitLayout();

private:
	void OnClearShapes(wxCommandEvent& event);
	void OnCreateBounding(wxCommandEvent& event);

private:
	StagePanel* m_stage_panel;

}; // ToolbarPanel

}

#endif // _LIBSHAPE_TOOLBAR_PANEL_H_
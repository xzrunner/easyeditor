#ifndef _EASYUI_LIST_TOOLBAR_PANEL_H_
#define _EASYUI_LIST_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eui
{
namespace list
{

class StagePanel;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage_panel);

protected:
	virtual wxSizer* initLayout();

private:
	void OnItemFilling(wxCommandEvent& event);
	void OnItemClear(wxCommandEvent& event);

	void OnChangeHori(wxCommandEvent& event);
	void OnChangeVert(wxCommandEvent& event);

private:
	StagePanel* m_stage_panel;

}; // ToolbarPanel

}
}

#endif // _EASYUI_LIST_TOOLBAR_PANEL_H_
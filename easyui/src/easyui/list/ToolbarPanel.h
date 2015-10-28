#ifndef _EASYUI_LIST_TOOLBAR_PANEL_H_
#define _EASYUI_LIST_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eui
{
namespace list
{

class UIList;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, UIList* list, d2d::EditPanel* editpanel);

	void EnableFillingBtn(bool enable);

protected:
	virtual wxSizer* initLayout();

private:
	void OnItemFilling(wxCommandEvent& event);

private:
	UIList* m_list;

	d2d::EditPanel* m_editpanel;

	wxButton* m_btn_filling;

}; // ToolbarPanel

}
}

#endif // _EASYUI_LIST_TOOLBAR_PANEL_H_
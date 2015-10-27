#ifndef _EASYUI_TOOLBAR_PANEL_H_
#define _EASYUI_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eui
{

class ToolbarPanel : public wxPanel
{
public:
	ToolbarPanel(wxWindow* parent);

	int AddToolbar(d2d::ToolbarPanel* toolbar);

	void EnableToolbar(int idx);

private:
	void InitLayout();

private:
	std::vector<d2d::ToolbarPanel*> m_toolbars;

}; // ToolbarPanel

}

#endif // _EASYUI_TOOLBAR_PANEL_H_
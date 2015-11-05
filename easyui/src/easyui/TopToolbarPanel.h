#ifndef _EASYUI_TOP_TOOLBAR_PANEL_H_
#define _EASYUI_TOP_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace eui
{

class TopToolbarPanel : public wxPanel
{
public:
	TopToolbarPanel(wxWindow* parent);

	int AddToolbar(d2d::ToolbarPanel* toolbar);

	void EnableToolbar(int idx);

private:
	void InitLayout();

private:
	std::vector<d2d::ToolbarPanel*> m_toolbars;

}; // TopToolbarPanel

}

#endif // _EASYUI_TOP_TOOLBAR_PANEL_H_
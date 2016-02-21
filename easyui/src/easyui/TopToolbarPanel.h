#ifndef _EASYUI_TOP_TOOLBAR_PANEL_H_
#define _EASYUI_TOP_TOOLBAR_PANEL_H_

#include <wx/panel.h>

#include <vector>

namespace ee { class ToolbarPanel; }

namespace eui
{

class TopToolbarPanel : public wxPanel
{
public:
	TopToolbarPanel(wxWindow* parent);

	int AddToolbar(ee::ToolbarPanel* toolbar);

	void EnableToolbar(int idx);

private:
	void InitLayout();

private:
	std::vector<ee::ToolbarPanel*> m_toolbars;

}; // TopToolbarPanel

}

#endif // _EASYUI_TOP_TOOLBAR_PANEL_H_
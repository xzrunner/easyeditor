#ifndef _EASYIMAGE_TOOLBAR_PANEL_H_
#define _EASYIMAGE_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace ee { class LibraryPanel; }

namespace eimage
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, ee::LibraryPanel* library, StagePanel* stage);

protected:
	virtual wxSizer* InitLayout() override;

}; // ToolbarPanel

}

#endif // _EASYIMAGE_TOOLBAR_PANEL_H_

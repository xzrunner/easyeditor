#ifndef _EASYMASK_TOOLBAR_PANEL_H_
#define _EASYMASK_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace emask
{

class StagePanel;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage);

protected:
	virtual wxSizer* InitLayout();

private:
	void OnChangeMaskRender(wxCommandEvent& event);

private:
	StagePanel* m_stage;

}; // ToolbarPanel

}

#endif // _EASYMASK_TOOLBAR_PANEL_H_
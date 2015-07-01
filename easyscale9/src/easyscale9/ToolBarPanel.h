#ifndef _ESCALE9_TOOLBAR_PANEL_H_
#define _ESCALE9_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace escale9
{

class StagePanel;
class ResizeCMPT;

class ToolbarPanel : public d2d::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage,
		d2d::PropertySettingPanel* property,
		d2d::ViewPanelMgr* view_panel_mgr);
	
	void setComposeOP(bool use) {
		m_isComposeOP = use;
	}
	bool isComposeOP() const {
		return m_isComposeOP;
	}

	void setSize(float width, float height);
	float getWidth() const;
	float getHeight() const;

protected:
	virtual wxSizer* initLayout();

private:
	ResizeCMPT* m_resizeCmpt;

	bool m_isComposeOP;

}; // ToolbarPanel

}

#endif // _ESCALE9_TOOLBAR_PANEL_H_
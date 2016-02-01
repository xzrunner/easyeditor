#ifndef _EASYSCALE9_TOOLBAR_PANEL_H_
#define _EASYSCALE9_TOOLBAR_PANEL_H_

#include <ee/ToolbarPanel.h>

namespace ee { class PropertySettingPanel; }

namespace escale9
{

class StagePanel;
class ResizeCMPT;

class ToolbarPanel : public ee::ToolbarPanel
{
public:
	ToolbarPanel(wxWindow* parent, StagePanel* stage,
		ee::PropertySettingPanel* property);
	
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
	virtual wxSizer* InitLayout();

private:
	ResizeCMPT* m_resizeCmpt;

	bool m_isComposeOP;

}; // ToolbarPanel

}

#endif // _EASYSCALE9_TOOLBAR_PANEL_H_
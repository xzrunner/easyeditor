#ifndef _ESCALE9_TOOLBAR_PANEL_H_
#define _ESCALE9_TOOLBAR_PANEL_H_



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

#endif // _ESCALE9_TOOLBAR_PANEL_H_
#ifndef SHOOTBUBBLE_TOOLBAR_PANEL_H_
#define SHOOTBUBBLE_TOOLBAR_PANEL_H_

#include <drag2d.h>

namespace shootbubble
{
	class StagePanel;

	class ToolbarPanel : public d2d::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent);

	protected:
		virtual wxSizer* initLayout();

	private:
		void onParamsChanged(wxSpinEvent& event);

	private:
		wxSpinCtrl *m_row, *m_col;
		wxSpinCtrl* m_edge;

	}; // ToolbarPanel
}

#endif // SHOOTBUBBLE_TOOLBAR_PANEL_H_
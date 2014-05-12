#ifndef FORMATION_TOOLBAR_PANEL_H
#define FORMATION_TOOLBAR_PANEL_H

#include <drag2d.h>

namespace formation
{
	class StagePanel;
	class RandomPasteCMPT;

	class ToolbarPanel : public d2d::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent);

		void initFromContext();

		void updateByLibrary();

	protected:
		virtual wxSizer* initLayout();

	private:
		void onParamsChanged(wxSpinEvent& event);

	private:
		wxSpinCtrl *m_width, *m_height;
		wxSpinCtrl* m_time;

		RandomPasteCMPT* m_randomPaste;

	}; // ToolbarPanel
}

#endif // FORMATION_TOOLBAR_PANEL_H
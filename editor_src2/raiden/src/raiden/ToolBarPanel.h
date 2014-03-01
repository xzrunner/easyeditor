#ifndef RAIDEN_TOOLBAR_PANEL_H
#define RAIDEN_TOOLBAR_PANEL_H

#include <drag2d.h>

namespace raiden
{
	class StagePanel;

	class ToolbarPanel : public d2d::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent);

	protected:
		virtual wxSizer* initLayout();

	private:
		wxSizer* initSettingLayout();

		void onSpeedChanged(wxSpinEvent& event);

	private:
		wxSpinCtrl* m_speed;

	}; // ToolbarPanel
}

#endif // RAIDEN_TOOLBAR_PANEL_H
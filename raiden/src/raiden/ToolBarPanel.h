#ifndef RAIDEN_TOOLBAR_PANEL_H
#define RAIDEN_TOOLBAR_PANEL_H



namespace raiden
{
	class StagePanel;

	class ToolbarPanel : public ee::ToolbarPanel
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
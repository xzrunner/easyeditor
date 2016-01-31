#ifndef FORMATION_TOOLBAR_PANEL_H
#define FORMATION_TOOLBAR_PANEL_H



namespace formation
{
	class StagePanel;
	class RandomPasteCMPT;

	class ToolbarPanel : public ee::ToolbarPanel
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
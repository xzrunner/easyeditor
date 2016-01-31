
#ifndef EMODELING_TOOLBAR_PANEL_H
#define EMODELING_TOOLBAR_PANEL_H



namespace emodeling
{
	class ToolbarPanel : public ee::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent);

	protected:
		virtual wxSizer* InitLayout();

	}; // ToolbarPanel
}

#endif // EMODELING_TOOLBAR_PANEL_H

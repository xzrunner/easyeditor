#pragma once



namespace coceditor
{
namespace complex
{
	class StagePanel;

	class ToolbarPanel : public ee::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent, StagePanel* editPanel);

	protected:
		virtual wxSizer* initLayout();

	}; // ToolbarPanel
}
}


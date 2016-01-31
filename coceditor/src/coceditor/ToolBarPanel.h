#pragma once



namespace coceditor
{
	class ToolbarPanel : public ee::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent);

	protected:
		virtual wxSizer* initLayout();

	}; // ToolbarPanel
}


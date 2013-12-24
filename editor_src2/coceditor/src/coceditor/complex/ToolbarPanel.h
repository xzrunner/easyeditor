#pragma once

#include <drag2d.h>

namespace coceditor
{
namespace complex
{
	class StagePanel;

	class ToolbarPanel : public d2d::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent, StagePanel* editPanel);

	protected:
		virtual wxSizer* initLayout();

	}; // ToolbarPanel
}
}


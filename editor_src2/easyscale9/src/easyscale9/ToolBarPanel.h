#pragma once

#include <drag2d.h>

namespace escale9
{
	class ToolbarPanel : public d2d::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent);

	protected:
		virtual wxSizer* initLayout();

	}; // ToolbarPanel
}


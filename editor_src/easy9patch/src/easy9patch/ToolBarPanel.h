#pragma once

#include <drag2d.h>

namespace e9patch
{
	class ToolbarPanel : public d2d::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent);

	protected:
		virtual wxSizer* initLayout();

	}; // ToolbarPanel
}


#pragma once

#include <drag2d.h>

namespace ecomplex
{
	class StagePanel;

	class ToolbarPanel : public d2d::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent, StagePanel* editPanel, 
			d2d::PropertySettingPanel* property);

	protected:
		virtual wxSizer* initLayout();

	}; // ToolbarPanel
}


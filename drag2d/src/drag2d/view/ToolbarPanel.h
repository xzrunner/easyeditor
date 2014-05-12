#pragma once

#include "component/AbstractEditCMPT.h"

namespace d2d
{
	class AbstractEditCMPT;

	class ToolbarPanel : public AbstractEditCMPT
	{
	public:
		ToolbarPanel(wxWindow* parent, EditPanel* editPanel, bool vertical = true);

	}; // ToolbarPanel
}


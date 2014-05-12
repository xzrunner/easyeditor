
#ifndef EMODELING_TOOLBAR_PANEL_H
#define EMODELING_TOOLBAR_PANEL_H

#include <drag2d.h>

namespace emodeling
{
	class ToolbarPanel : public d2d::ToolbarPanel
	{
	public:
		ToolbarPanel(wxWindow* parent);

	protected:
		virtual wxSizer* initLayout();

	}; // ToolbarPanel
}

#endif // EMODELING_TOOLBAR_PANEL_H


#ifndef EBUILDER_COLLAPSIBLE_PANEL_H
#define EBUILDER_COLLAPSIBLE_PANEL_H

#include <wx/wx.h>
#include <wx/collpane.h>

namespace ebuilder
{
	class CollapsiblePane : public wxCollapsiblePane
	{
	public:
		CollapsiblePane(wxWindow* parent, const wxString& name);

		virtual void Collapse(bool collapse = true);

	private:
		void refresh();

	protected:
		wxWindow* m_parent;

	}; // CollapsiblePane
}

#endif // EBUILDER_COLLAPSIBLE_PANEL_H

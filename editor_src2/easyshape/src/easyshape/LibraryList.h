
#ifndef ESHAPE_LIBRARY_LIST_H
#define ESHAPE_LIBRARY_LIST_H

#include <drag2d.h>

namespace eshape
{
	class StagePanel;
	class ToolbarPanel;

	class LibraryList : public d2d::LibraryList
	{
	public:
		LibraryList(wxWindow* parent);

		virtual void onListSelected(wxCommandEvent& event);

		void setContext(StagePanel* stage, ToolbarPanel* toolbar) {
			m_stage = stage;
			m_toolbar = toolbar;
		}

	private:
		StagePanel* m_stage;
		ToolbarPanel* m_toolbar;

	}; // LibraryList
}

#endif // ESHAPE_LIBRARY_LIST_H

#ifndef ESHAPE_LIBRARY_PAGE_H
#define ESHAPE_LIBRARY_PAGE_H

#include <drag2d.h>

namespace eshape
{
	class StagePanel;
	class ToolbarPanel;
	class LibraryItem;

	class LibraryPage : public d2d::ILibraryPage
	{
	public:
		LibraryPage(wxWindow* parent, const char* name);

		void clickListSelection();

		void setContext(StagePanel* stage, ToolbarPanel* toolbar);

	private:
		virtual void onDelPress(wxCommandEvent& event);

	protected:
		StagePanel* m_stage;
		ToolbarPanel* m_toolbar;

	}; // LibraryPage
}

#endif // ESHAPE_LIBRARY_PAGE_H

#ifndef ESHAPE_LIBRARY_PAGE_H
#define ESHAPE_LIBRARY_PAGE_H

#include <drag2d.h>

namespace eshape
{
	class LibraryItem;

	class LibraryPage : public d2d::ILibraryPage
	{
	public:
		LibraryPage(wxWindow* parent, const char* name);

		void clickListSelection();

	private:
		virtual void onDelPress(wxCommandEvent& event);

	}; // LibraryPage
}

#endif // ESHAPE_LIBRARY_PAGE_H
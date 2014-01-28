#ifndef COMPLEX_LIBRARY_PAGE_H
#define COMPLEX_LIBRARY_PAGE_H

#include <drag2d.h>

namespace complex
{
	class LibraryPage : public d2d::ILibraryPage
	{
	public:
		LibraryPage(wxWindow* parent);

		virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	}; // LibraryPage
}

#endif // COMPLEX_LIBRARY_PAGE_H

#ifndef ESHAPE_LIBRARY_IMAGE_PAGE_H
#define ESHAPE_LIBRARY_IMAGE_PAGE_H

#include "LibraryPage.h"

namespace eshape
{
	class LibraryImagePage : public LibraryPage
	{
	public:
		LibraryImagePage(wxWindow* parent);

		virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

	protected:
		virtual void onAddPress(wxCommandEvent& event);

	}; // LibraryImagePage
}

#endif // ESHAPE_LIBRARY_IMAGE_PAGE_H
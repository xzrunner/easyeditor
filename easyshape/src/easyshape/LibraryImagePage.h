#ifndef _ESHAPE_LIBRARY_IMAGE_PAGE_H_
#define _ESHAPE_LIBRARY_IMAGE_PAGE_H_

#include "LibraryPage.h"

namespace eshape
{

class LibraryImagePage : public LibraryPage
{
public:
	LibraryImagePage(wxWindow* parent);

	virtual bool isHandleSymbol(ee::ISymbol* symbol) const;

protected:
	virtual void onAddPress(wxCommandEvent& event);

}; // LibraryImagePage

}

#endif // _ESHAPE_LIBRARY_IMAGE_PAGE_H_
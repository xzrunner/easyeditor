#ifndef _EASYTERRAIN2D_LIBRARY_PAGE_H_
#define _EASYTERRAIN2D_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace eterrain2d
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(ee::Symbol* symbol) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LibraryPage

}

#endif // _EASYTERRAIN2D_LIBRARY_PAGE_H_
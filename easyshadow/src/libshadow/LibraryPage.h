#ifndef _EASYSHADOW_LIBRARY_PAGE_H_
#define _EASYSHADOW_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace eshadow
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(ee::Symbol* sym) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LibraryPage

}

#endif // _EASYSHADOW_LIBRARY_PAGE_H_
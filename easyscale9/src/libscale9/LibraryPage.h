#ifndef _EASYSCALE9_LIBRARY_PAGE_H_
#define _EASYSCALE9_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace escale9
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

#endif // _EASYSCALE9_LIBRARY_PAGE_H_
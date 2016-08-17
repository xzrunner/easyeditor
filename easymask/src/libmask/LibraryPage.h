#ifndef _EASYMASK_LIBRARY_PAGE_H_
#define _EASYMASK_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace emask
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

#endif // _EASYMASK_LIBRARY_PAGE_H_
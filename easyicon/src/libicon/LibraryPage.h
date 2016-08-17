#ifndef _EASYICON_LIBRARY_PAGE_H_
#define _EASYICON_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace eicon
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

#endif // _EASYICON_LIBRARY_PAGE_H_
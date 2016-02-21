#ifndef _EASYP3DINV_LIBRARY_PAGE_H_
#define _EASYP3DINV_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace ep3dinv
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

#endif // _EASYP3DINV_LIBRARY_PAGE_H_
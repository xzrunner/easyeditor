#ifndef _EASYBONE_LIBRARY_PAGE_H_
#define _EASYBONE_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace ebone
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

#endif // _EASYBONE_LIBRARY_PAGE_H_

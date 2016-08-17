#ifndef _EASYEJOY2D_LIBRARY_PAGE_H_
#define _EASYEJOY2D_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace eejoy2d
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

#endif // _EASYEJOY2D_LIBRARY_PAGE_H_
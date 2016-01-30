#ifndef _EASYICON_LIBRARY_PAGE_H_
#define _EASYICON_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace eicon
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(d2d::Symbol* symbol) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LibraryPage

}

#endif // _EASYICON_LIBRARY_PAGE_H_
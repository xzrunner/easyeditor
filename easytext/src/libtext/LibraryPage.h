#ifndef _EASYTEXT_LIBRARY_PAGE_H_
#define _EASYTEXT_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace etext
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(d2d::ISymbol* symbol) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LibraryPage

}

#endif // _EASYTEXT_LIBRARY_PAGE_H_
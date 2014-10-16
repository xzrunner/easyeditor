#ifndef _LIBSKETCH_LIBRARY_PAGE_H_
#define _LIBSKETCH_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace libsketch
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool isHandleSymbol(d2d::ISymbol* symbol) const;

protected:
	virtual void onAddPress(wxCommandEvent& event);

}; // LibraryPage

}

#endif // _LIBSKETCH_LIBRARY_PAGE_H_
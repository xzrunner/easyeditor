#ifndef _EASYTEXTURE_LIBRARY_PAGE_H_
#define _EASYTEXTURE_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace etexture
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

#endif // _EASYTEXTURE_LIBRARY_PAGE_H_

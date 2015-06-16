#ifndef _SG_LIBRARY_PAGE_H_
#define _SG_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace sg
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent, const char* name);

	virtual bool IsHandleSymbol(d2d::ISymbol* symbol) const;

}; // LibraryPage 

}

#endif // _SG_LIBRARY_PAGE_H_

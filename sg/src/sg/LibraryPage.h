#ifndef _SG_LIBRARY_PAGE_H_
#define _SG_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace sg
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent, const char* name);

	virtual bool IsHandleSymbol(ee::Symbol* sym) const;

}; // LibraryPage 

}

#endif // _SG_LIBRARY_PAGE_H_

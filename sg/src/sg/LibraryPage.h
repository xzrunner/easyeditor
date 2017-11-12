#ifndef _SG_LIBRARY_PAGE_H_
#define _SG_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace sg
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent, const char* name);

	virtual bool IsHandleSymbol(const ee::SymPtr& sym) const override;

}; // LibraryPage 

}

#endif // _SG_LIBRARY_PAGE_H_

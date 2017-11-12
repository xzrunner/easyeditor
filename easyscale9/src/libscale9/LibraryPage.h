#ifndef _EASYSCALE9_LIBRARY_PAGE_H_
#define _EASYSCALE9_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace escale9
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(const ee::SymPtr& sym) const override;

protected:
	virtual void OnAddPress(wxCommandEvent& event) override;

}; // LibraryPage

}

#endif // _EASYSCALE9_LIBRARY_PAGE_H_
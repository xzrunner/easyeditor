#ifndef _EASYSHADOW_LIBRARY_PAGE_H_
#define _EASYSHADOW_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace eshadow
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

#endif // _EASYSHADOW_LIBRARY_PAGE_H_
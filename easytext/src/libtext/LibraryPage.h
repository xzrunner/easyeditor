#ifndef _EASYTEXT_LIBRARY_PAGE_H_
#define _EASYTEXT_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace etext
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

#endif // _EASYTEXT_LIBRARY_PAGE_H_
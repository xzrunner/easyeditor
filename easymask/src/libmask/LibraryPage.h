#ifndef _EASYMASK_LIBRARY_PAGE_H_
#define _EASYMASK_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace emask
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

#endif // _EASYMASK_LIBRARY_PAGE_H_
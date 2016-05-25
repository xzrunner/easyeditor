#ifndef _EASYMASK_LIBRARY_PAGE_H_
#define _EASYMASK_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace emask
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent, const std::string& name);

	virtual bool IsHandleSymbol(ee::Symbol* symbol) const { return true; }

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LibraryPage

}

#endif // _EASYMASK_LIBRARY_PAGE_H_
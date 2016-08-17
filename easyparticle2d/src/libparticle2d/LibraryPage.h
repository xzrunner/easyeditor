#ifndef _EASYPARTICLE2D_LIBRARY_PAGE_H_
#define _EASYPARTICLE2D_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace eparticle2d
{

class LibraryPage : public ee::LibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(ee::Symbol* sym) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LibraryPage

}

#endif // _EASYPARTICLE2D_LIBRARY_PAGE_H_
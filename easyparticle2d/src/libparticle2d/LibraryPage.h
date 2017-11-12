#ifndef _EASYPARTICLE2D_LIBRARY_PAGE_H_
#define _EASYPARTICLE2D_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace eparticle2d
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

#endif // _EASYPARTICLE2D_LIBRARY_PAGE_H_
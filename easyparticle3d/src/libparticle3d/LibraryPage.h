#ifndef _EASYPARTICLE3D_LIBRARY_PAGE_H_
#define _EASYPARTICLE3D_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace eparticle3d
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

#endif // _EASYPARTICLE3D_LIBRARY_PAGE_H_
#ifndef _EASYPSEUDO3D_LIBRARY_PAGE_H_
#define _EASYPSEUDO3D_LIBRARY_PAGE_H_

#include <ee/LibraryPage.h>

namespace epseudo3d
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

#endif // _EASYPSEUDO3D_LIBRARY_PAGE_H_

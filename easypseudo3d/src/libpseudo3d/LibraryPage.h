#ifndef _EASYPSEUDO3D_LIBRARY_PAGE_H_
#define _EASYPSEUDO3D_LIBRARY_PAGE_H_

#include <drag2d.h>

namespace epseudo3d
{

class LibraryPage : public d2d::ILibraryPage
{
public:
	LibraryPage(wxWindow* parent);

	virtual bool IsHandleSymbol(d2d::Symbol* symbol) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LibraryPage

}

#endif // _EASYPSEUDO3D_LIBRARY_PAGE_H_

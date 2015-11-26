#ifndef D2D_LIBRARY_SCRIPTS_PAGE_H
#define D2D_LIBRARY_SCRIPTS_PAGE_H

#include "ILibraryPage.h"

namespace d2d
{

class LibraryScriptsPage : public ILibraryPage
{
public:
	LibraryScriptsPage(wxWindow* parent);

	virtual bool IsHandleSymbol(d2d::ISymbol* symbol) const;

protected:
	virtual void OnAddPress(wxCommandEvent& event);

}; // LibraryScriptsPage

}

#endif // D2D_LIBRARY_SCRIPTS_PAGE_H
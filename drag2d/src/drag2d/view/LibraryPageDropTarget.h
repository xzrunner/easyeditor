#ifndef _DRAG2D_LIBRARY_PAGE_DROP_TARGET_H_
#define _DRAG2D_LIBRARY_PAGE_DROP_TARGET_H_

#include <wx/dnd.h>

namespace d2d
{

class ILibraryPage;

class LibraryPageDropTarget : public wxFileDropTarget
{
public:
	LibraryPageDropTarget(ILibraryPage* page) 
		: m_page(page) {}

	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

private:
	ILibraryPage* m_page;

}; // LibraryPageDropTarget

}

#endif // _DRAG2D_LIBRARY_PAGE_DROP_TARGET_H_
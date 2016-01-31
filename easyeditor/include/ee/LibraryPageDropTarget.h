#ifndef _EASYEDITOR_LIBRARY_PAGE_DROP_TARGET_H_
#define _EASYEDITOR_LIBRARY_PAGE_DROP_TARGET_H_

#include <wx/dnd.h>

namespace ee
{

class LibraryPage;

class LibraryPageDropTarget : public wxFileDropTarget
{
public:
	LibraryPageDropTarget(LibraryPage* page) 
		: m_page(page) {}

	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

private:
	LibraryPage* m_page;

}; // LibraryPageDropTarget

}

#endif // _EASYEDITOR_LIBRARY_PAGE_DROP_TARGET_H_
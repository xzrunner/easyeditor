#ifndef _DRAG2D_LIBRARY_DROP_TARGET_H_
#define _DRAG2D_LIBRARY_DROP_TARGET_H_

#include <wx/dnd.h>

namespace d2d
{

class LibraryPanel;

class LibraryDropTarget : public wxFileDropTarget
{
public:
	LibraryDropTarget(LibraryPanel* library) : m_library(library) {}
	
	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

private:
	LibraryPanel* m_library;

}; // LibraryDropTarget 

}

#endif // _DRAG2D_LIBRARY_DROP_TARGET_H_
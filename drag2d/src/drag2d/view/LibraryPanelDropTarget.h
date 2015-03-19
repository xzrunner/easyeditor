#ifndef _DRAG2D_LIBRARY_PANEL_DROP_TARGET_H_
#define _DRAG2D_LIBRARY_PANEL_DROP_TARGET_H_

#include <wx/dnd.h>

namespace d2d
{

class LibraryPanel;

class LibraryPanelDropTarget : public wxFileDropTarget
{
public:
	LibraryPanelDropTarget(LibraryPanel* panel) 
		: m_panel(panel) {}
	
	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

private:
	LibraryPanel* m_panel;

}; // LibraryPanelDropTarget 

}

#endif // _DRAG2D_LIBRARY_PANEL_DROP_TARGET_H_
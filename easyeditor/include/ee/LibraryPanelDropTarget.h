#ifndef _EASYEDITOR_LIBRARY_PANEL_DROP_TARGET_H_
#define _EASYEDITOR_LIBRARY_PANEL_DROP_TARGET_H_

#include <wx/dnd.h>

namespace ee
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

#endif // _EASYEDITOR_LIBRARY_PANEL_DROP_TARGET_H_
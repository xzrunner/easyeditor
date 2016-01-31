#ifndef _EASYEDITOR_STAGE_DROP_TARGET_H_
#define _EASYEDITOR_STAGE_DROP_TARGET_H_

#include "CombinedDropTarget.h"

namespace ee
{

class EditPanelImpl;
class LibraryPanel;
class Symbol;
class Vector;

class StageDropTarget : public CombinedDropTarget
{
public:
	StageDropTarget(wxWindow* stage_wnd, EditPanelImpl* stage, 
		LibraryPanel* library);

	virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text);
	virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

private:
	EditPanelImpl* m_stage;
	LibraryPanel* m_library;

}; // StageDropTarget 

}

#endif // _EASYEDITOR_STAGE_DROP_TARGET_H_
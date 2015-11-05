#ifndef _DRAG2D_STAGE_DROP_TARGET_H_
#define _DRAG2D_STAGE_DROP_TARGET_H_

#include "CombinedDropTarget.h"

namespace d2d
{

class EditPanelImpl;
class LibraryPanel;
class ISymbol;
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

#endif // _DRAG2D_STAGE_DROP_TARGET_H_
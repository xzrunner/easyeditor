#ifndef _EASYEDITOR_DRAG_TARGET_NULL_H_
#define _EASYEDITOR_DRAG_TARGET_NULL_H_

#include <wx/dnd.h>

namespace ee
{

class DragTargetNull : public wxTextDropTarget
{
public:
	virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data) {
		return true;
	}

}; // DragTargetNull

}

#endif // _EASYEDITOR_DRAG_TARGET_NULL_H_
#pragma once

#include <wx/dnd.h>

namespace d2d
{
	class DragTargetNull : public wxTextDropTarget
	{
	public:
		virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data) {
			return true;
		}

	}; // DragTargetNull
}


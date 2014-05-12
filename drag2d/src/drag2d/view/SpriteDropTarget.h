#pragma once

#include <wx/dnd.h>

namespace d2d
{
	class SpritesPanelImpl;
	class EditPanel;
	class LibraryPanel;

	class SpriteDropTarget : public wxTextDropTarget
	{
	public:
		SpriteDropTarget(SpritesPanelImpl* panelImpl, EditPanel* editPanel,
			LibraryPanel* libraryPanel);

		virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

	private:
		SpritesPanelImpl* m_panelImpl;
		EditPanel* m_editPanel;
		LibraryPanel* m_libraryPanel;

	}; // SpriteDropTarget
}
#ifndef _DRAG2D_SPRITE_DROP_TARGET_H_
#define _DRAG2D_SPRITE_DROP_TARGET_H_

#include <wx/dnd.h>

namespace d2d
{

class EditPanelImpl;
class LibraryPanel;

class SpriteDropTarget : public wxTextDropTarget
{
public:
	SpriteDropTarget(EditPanelImpl* stage, LibraryPanel* libraryPanel);

	virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

private:
	EditPanelImpl* m_stage;
	LibraryPanel* m_libraryPanel;

}; // SpriteDropTarget

}

#endif // _DRAG2D_SPRITE_DROP_TARGET_H_
#ifndef _DRAG2D_SPRITE_DROP_TARGET_H_
#define _DRAG2D_SPRITE_DROP_TARGET_H_

#include <wx/dnd.h>

namespace d2d
{

class MultiSpritesImpl;
class EditPanelImpl;
class LibraryPanel;

class SpriteDropTarget : public wxTextDropTarget
{
public:
	SpriteDropTarget(MultiSpritesImpl* panelImpl, EditPanelImpl* stage,
		LibraryPanel* libraryPanel);

	virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

private:
	MultiSpritesImpl* m_panelImpl;
	EditPanelImpl* m_stage;
	LibraryPanel* m_libraryPanel;

}; // SpriteDropTarget

}

#endif // _DRAG2D_SPRITE_DROP_TARGET_H_
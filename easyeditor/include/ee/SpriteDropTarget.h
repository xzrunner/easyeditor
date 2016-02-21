#ifndef _EASYEDITOR_SPRITE_DROP_TARGET_H_
#define _EASYEDITOR_SPRITE_DROP_TARGET_H_

#include <wx/dnd.h>

namespace ee
{

class EditPanelImpl;
class LibraryPanel;

class SpriteDropTarget : public wxTextDropTarget
{
public:
	SpriteDropTarget(EditPanelImpl* stage, LibraryPanel* library);

	virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

private:
	EditPanelImpl* m_stage;
	LibraryPanel* m_library;

}; // SpriteDropTarget

}

#endif // _EASYEDITOR_SPRITE_DROP_TARGET_H_
#ifndef _EASYUI_WINDOW_SPRITE_DROP_TARGET_H_
#define _EASYUI_WINDOW_SPRITE_DROP_TARGET_H_

#include <wx/dnd.h>

namespace ee { class EditPanelImpl; class LibraryPanel; }

namespace eui
{
namespace window
{

class SpriteDropTarget : public wxTextDropTarget
{
public:
	SpriteDropTarget(ee::EditPanelImpl* stage, ee::LibraryPanel* library);

	virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data);

private:
	ee::EditPanelImpl* m_stage;
	ee::LibraryPanel* m_library;

}; // SpriteDropTarget

}
}

#endif // _EASYUI_WINDOW_SPRITE_DROP_TARGET_H_
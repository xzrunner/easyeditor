#ifndef _DRAG2D_STAGE_DROP_TARGET_H_
#define _DRAG2D_STAGE_DROP_TARGET_H_

#include "CombinedDropTarget.h"

namespace d2d
{

class EditPanel;
class MultiSpritesImpl;
class LibraryPanel;
class ISymbol;
class Vector;

class StageDropTarget : public CombinedDropTarget
{
public:
	StageDropTarget(EditPanel* edit_panel, MultiSpritesImpl* sprites_impl, 
		LibraryPanel* library);

	virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text);
	virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

private:
	EditPanel* m_edit_panel;
	MultiSpritesImpl* m_sprites_impl;
	LibraryPanel* m_library;

}; // StageDropTarget 

}

#endif // _DRAG2D_STAGE_DROP_TARGET_H_
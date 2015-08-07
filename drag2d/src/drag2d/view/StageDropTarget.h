#ifndef _DRAG2D_STAGE_DROP_TARGET_H_
#define _DRAG2D_STAGE_DROP_TARGET_H_

#include "CombinedDropTarget.h"

namespace d2d
{

class EditPanelImpl;
class MultiSpritesImpl;
class LibraryPanel;
class ISymbol;
class Vector;

class StageDropTarget : public CombinedDropTarget
{
public:
	StageDropTarget(wxWindow* stage_wnd, EditPanelImpl* stage, 
		MultiSpritesImpl* sprites_impl, LibraryPanel* library);

	virtual void OnDropText(wxCoord x, wxCoord y, const wxString& text);
	virtual void OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

private:
	EditPanelImpl* m_stage;
	MultiSpritesImpl* m_sprites_impl;
	LibraryPanel* m_library;

}; // StageDropTarget 

}

#endif // _DRAG2D_STAGE_DROP_TARGET_H_
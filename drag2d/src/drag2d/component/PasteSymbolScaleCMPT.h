#ifndef _DRAG2D_PASTE_SYMBOL_SCALE_CMPT_H_
#define _DRAG2D_PASTE_SYMBOL_SCALE_CMPT_H_

#include "AbstractEditCMPT.h"

namespace d2d
{

class MultiSpritesImpl;
class LibraryPanel;

class PasteSymbolScaleCMPT : public AbstractEditCMPT
{
public:
	PasteSymbolScaleCMPT(wxWindow* parent, const wxString& name,
		wxWindow* stage_wnd, EditPanelImpl* stage, 
		MultiSpritesImpl* spritesImpl, LibraryPanel* libraryPanel);

protected:
	virtual wxSizer* initLayout();

private:
	void onChangeScale(wxSpinEvent& event);

private:
	float m_scaleVal;

}; // PasteSymbolScaleCMPT

}

#endif // _DRAG2D_PASTE_SYMBOL_SCALE_CMPT_H_
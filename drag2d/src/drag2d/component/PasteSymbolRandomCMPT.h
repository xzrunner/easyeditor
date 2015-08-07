#ifndef _DRAG2D_PASTE_SYMBOL_RANDOM_CMPT_H_
#define _DRAG2D_PASTE_SYMBOL_RANDOM_CMPT_H_

#include "AbstractEditCMPT.h"

namespace d2d
{

class MultiSpritesImpl;
class LibraryPanel;
class PasteSymbolRandomWidget;

class PasteSymbolRandomCMPT : public AbstractEditCMPT
{
public:
	PasteSymbolRandomCMPT(wxWindow* parent, const wxString& name,
		wxWindow* stage_wnd, EditPanelImpl* stage, 
		MultiSpritesImpl* spritesImpl, LibraryPanel* libraryPanel);

protected:
	virtual wxSizer* initLayout();

private:
	PasteSymbolRandomWidget* m_randomWidget;

}; // PasteSymbolRandomCMPT

}

#endif // _DRAG2D_PASTE_SYMBOL_RANDOM_CMPT_H_

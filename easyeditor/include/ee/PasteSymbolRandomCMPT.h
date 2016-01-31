#ifndef _EASYEDITOR_PASTE_SYMBOL_RANDOM_CMPT_H_
#define _EASYEDITOR_PASTE_SYMBOL_RANDOM_CMPT_H_

#include "EditCMPT.h"

namespace ee
{

class MultiSpritesImpl;
class LibraryPanel;
class PasteSymbolRandomWidget;

class PasteSymbolRandomCMPT : public EditCMPT
{
public:
	PasteSymbolRandomCMPT(wxWindow* parent, const std::string& name,
		wxWindow* stage_wnd, EditPanelImpl* stage, 
		MultiSpritesImpl* spritesImpl, LibraryPanel* libraryPanel);

protected:
	virtual wxSizer* InitLayout();

private:
	PasteSymbolRandomWidget* m_random_widget;

}; // PasteSymbolRandomCMPT

}

#endif // _EASYEDITOR_PASTE_SYMBOL_RANDOM_CMPT_H_

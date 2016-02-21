#ifndef _EASYEDITOR_PASTE_SYMBOL_OFFSET_CMPT_H_
#define _EASYEDITOR_PASTE_SYMBOL_OFFSET_CMPT_H_

#include "EditCMPT.h"
#include "Vector.h"

namespace ee
{

class MultiSpritesImpl;
class LibraryPanel;

template <typename TOperation>
class PasteSymbolOffsetCMPT : public EditCMPT
{
public:
	PasteSymbolOffsetCMPT(wxWindow* parent, const std::string& name,
		wxWindow* stage_wnd, EditPanelImpl* stage, 
		MultiSpritesImpl* sprites_impl, LibraryPanel* library);

	Vector GetOffset() const;

protected:
	virtual wxSizer* InitLayout();

private:
	wxSpinCtrl *m_xspin, *m_yspin;

}; // PasteSymbolOffsetCMPT

}

#include "PasteSymbolOffsetCMPT.inl"

#endif // _EASYEDITOR_PASTE_SYMBOL_OFFSET_CMPT_H_
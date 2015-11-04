#ifndef _DRAG2D_PASTE_SPRITE_CMPT_H_
#define _DRAG2D_PASTE_SPRITE_CMPT_H_

#include "AbstractEditCMPT.h"

namespace d2d
{

class MultiSpritesImpl;

class PasteSpriteCMPT : public AbstractEditCMPT
{
public:
	PasteSpriteCMPT(wxWindow* parent, const wxString& name, wxWindow* stage_wnd, 
		EditPanelImpl* stage, MultiSpritesImpl* spritesImpl);

	bool isHorMirror() const { return m_xMirror->IsChecked(); }
	bool isVerMirror() const { return m_yMirror->IsChecked(); }

protected:
	virtual wxSizer* initLayout();

private:
	wxCheckBox* m_xMirror;
	wxCheckBox* m_yMirror;

}; // PasteSpriteCMPT

}

#endif // _DRAG2D_PASTE_SPRITE_CMPT_H_
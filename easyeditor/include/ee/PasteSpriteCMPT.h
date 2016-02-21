#ifndef _EASYEDITOR_PASTE_SPRITE_CMPT_H_
#define _EASYEDITOR_PASTE_SPRITE_CMPT_H_

#include "EditCMPT.h"

namespace ee
{

class MultiSpritesImpl;

class PasteSpriteCMPT : public EditCMPT
{
public:
	PasteSpriteCMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd, 
		EditPanelImpl* stage, MultiSpritesImpl* sprites_impl);

	bool isHorMirror() const { return m_xMirror->IsChecked(); }
	bool isVerMirror() const { return m_yMirror->IsChecked(); }

protected:
	virtual wxSizer* InitLayout();

private:
	wxCheckBox* m_xMirror;
	wxCheckBox* m_yMirror;

}; // PasteSpriteCMPT

}

#endif // _EASYEDITOR_PASTE_SPRITE_CMPT_H_
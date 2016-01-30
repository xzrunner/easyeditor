#ifndef _EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_
#define _EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_

#include <drag2d.h>

namespace ecomplex
{

class OpenSymbolDialog
{
public:
	OpenSymbolDialog(wxWindow* wnd, d2d::EditPanelImpl* stage,
		d2d::MultiSpritesImpl* sprites_impl);

	void Open(d2d::Sprite* spr);

private:
	wxWindow* m_wnd;

	d2d::EditPanelImpl* m_stage;

	d2d::MultiSpritesImpl* m_sprites_impl;

}; // OpenSymbolDialog

}

#endif // _EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_
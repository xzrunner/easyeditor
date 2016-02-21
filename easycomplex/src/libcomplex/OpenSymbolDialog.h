#ifndef _EASY_EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_
#define _EASY_EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_

namespace ee { class EditPanelImpl; class MultiSpritesImpl; class Sprite; }

class wxWindow;

namespace ecomplex
{

class OpenSymbolDialog
{
public:
	OpenSymbolDialog(wxWindow* wnd, ee::EditPanelImpl* stage,
		ee::MultiSpritesImpl* sprites_impl);

	void Open(ee::Sprite* spr);

private:
	wxWindow* m_wnd;

	ee::EditPanelImpl* m_stage;

	ee::MultiSpritesImpl* m_sprites_impl;

}; // OpenSymbolDialog

}

#endif // _EASY_EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_
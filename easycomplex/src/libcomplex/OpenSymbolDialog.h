#ifndef _EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_
#define _EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_



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

#endif // _EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_
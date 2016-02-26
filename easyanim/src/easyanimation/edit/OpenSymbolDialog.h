#ifndef _EASYANIM_OPEN_SYMBOL_DIALOG_H_
#define _EASYANIM_OPEN_SYMBOL_DIALOG_H_

class wxWindow;

namespace ee { class EditPanelImpl; class MultiSpritesImpl; class Sprite; }

namespace eanim
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

#endif // _EASYANIM_OPEN_SYMBOL_DIALOG_H_
#ifndef _EASY_EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_
#define _EASY_EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_

#include <vector>

#include <stddef.h>

namespace ee { class EditPanelImpl; class MultiSpritesImpl; class Sprite; class CrossGuides; }

class wxWindow;

namespace ecomplex
{

class OpenSymbolLsn;

class OpenSymbolDialog
{
public:
	OpenSymbolDialog(wxWindow* wnd, ee::EditPanelImpl* stage,
		ee::MultiSpritesImpl* sprites_impl);

	void Open(ee::Sprite* spr, ee::CrossGuides* guides = NULL);

	void RegistLsn(OpenSymbolLsn* lsn) {
		m_lsns.push_back(lsn);
	}

private:
	wxWindow* m_wnd;

	ee::EditPanelImpl* m_stage;

	ee::MultiSpritesImpl* m_sprites_impl;

	std::vector<OpenSymbolLsn*> m_lsns;

}; // OpenSymbolDialog

}

#endif // _EASY_EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_
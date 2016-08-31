#ifndef _EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_
#define _EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_

#include <vector>

#include <stddef.h>

namespace ee { class EditPanelImpl; class MultiSpritesImpl; class Sprite; class CrossGuides; class ViewlistPanel; }

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

	// todo
	void SetViewlist(ee::ViewlistPanel* viewlist) { m_viewlist = viewlist; }

private:
	wxWindow* m_wnd;

	ee::EditPanelImpl* m_stage;

	ee::MultiSpritesImpl* m_sprites_impl;

	ee::ViewlistPanel* m_viewlist;

	std::vector<OpenSymbolLsn*> m_lsns;

}; // OpenSymbolDialog

}

#endif // _EASYCOMPLEX_OPEN_SYMBOL_DIALOG_H_
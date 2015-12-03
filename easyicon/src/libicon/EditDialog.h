#ifndef _EASYICON_EDIT_DIALOG_H_
#define _EASYICON_EDIT_DIALOG_H_

#include <drag2d.h>

namespace eicon
{

class Symbol;
class StagePanel;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, wxGLContext* glctx,
		d2d::ISprite* edited, const d2d::MultiSpritesImpl* sprite_impl);

private:
	void InitLayout(wxGLContext* glctx, d2d::ISprite* edited, 
		const d2d::MultiSpritesImpl* sprite_impl);
	void InitEditOP(d2d::ISprite* edited);

	void OnCloseEvent(wxCloseEvent& event);

private:
	StagePanel* m_stage;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYICON_EDIT_DIALOG_H_
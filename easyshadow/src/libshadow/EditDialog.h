#ifndef _EASYSHADOW_EDIT_DIALOG_H_
#define _EASYSHADOW_EDIT_DIALOG_H_

#include <drag2d.h>

namespace eshadow
{

class Symbol;
class Sprite;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, wxGLContext* glctx,
		Sprite* edited, const d2d::MultiSpritesImpl* sprite_impl);
	virtual ~EditDialog();

private:
	void InitLayout(wxGLContext* glctx, d2d::Sprite* edited, 
		const d2d::MultiSpritesImpl* sprite_impl);

	void OnCloseEvent(wxCloseEvent& event);

	void InitCamera(d2d::Camera* cam, d2d::Sprite* spr) const;

private:
	Symbol* m_symbol;

	d2d::EditPanel* m_stage;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYSHADOW_EDIT_DIALOG_H_
#ifndef _EASYTERRAIN2D_EDIT_DIALOG_H_
#define _EASYTERRAIN2D_EDIT_DIALOG_H_

#include <drag2d.h>

namespace eterrain2d
{

class Symbol;
class Sprite;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, Sprite* edited, 
		const d2d::MultiSpritesImpl* sprite_impl);
	virtual ~EditDialog();

private:
	void InitLayout(d2d::ISprite* edited, 
		const d2d::MultiSpritesImpl* sprite_impl);

	void OnCloseEvent(wxCloseEvent& event);

private:
	Symbol* m_symbol;

	d2d::EditPanel* m_stage;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYTERRAIN2D_EDIT_DIALOG_H_
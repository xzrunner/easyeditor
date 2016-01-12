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
	EditDialog(wxWindow* parent, wxGLContext* glctx, 
		Sprite* edited, const d2d::MultiSpritesImpl* sprite_impl);
	virtual ~EditDialog();

private:
	void InitLayout(wxGLContext* glctx, d2d::ISprite* edited, 
		const d2d::MultiSpritesImpl* sprite_impl);

	void OnCloseEvent(wxCloseEvent& event);

	void InitCamera(d2d::Camera* cam, d2d::ISprite* spr) const;

private:
	Symbol* m_symbol;

	d2d::EditPanel* m_stage;

	bool m_visible_tex_edge;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYTERRAIN2D_EDIT_DIALOG_H_
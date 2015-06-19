#ifndef _DRAG2D_SPRITE_DIALOG_H_
#define _DRAG2D_SPRITE_DIALOG_H_

#include "BaseDialog.h"

namespace d2d
{

class ISprite;

class SpriteDialog : public BaseDialog
{
public:
	SpriteDialog(wxWindow* parent, ISprite* spr);

	wxString GetNameStr() const { return m_name_ctrl->GetValue(); }
	wxString GetTagStr() const { return m_tag_ctrl->GetValue(); }

private:
	void InitLayout();

private:
	ISprite* m_spr;

	wxTextCtrl* m_name_ctrl;
	wxTextCtrl* m_tag_ctrl;

}; // SpriteDialog

}

#endif // _DRAG2D_SPRITE_DIALOG_H_
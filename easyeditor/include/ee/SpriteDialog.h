#ifndef _EASYEDITOR_SPRITE_DIALOG_H_
#define _EASYEDITOR_SPRITE_DIALOG_H_

#include "BaseDialog.h"

namespace ee
{

class Sprite;

class SpriteDialog : public BaseDialog
{
public:
	SpriteDialog(wxWindow* parent, Sprite* spr);

	std::string GetNameStr() const { return m_name_ctrl->GetValue().ToStdString(); }
	std::string GetTagStr() const { return m_tag_ctrl->GetValue().ToStdString(); }

private:
	void InitLayout();

private:
	Sprite* m_spr;

	wxTextCtrl* m_name_ctrl;
	wxTextCtrl* m_tag_ctrl;

}; // SpriteDialog

}

#endif // _EASYEDITOR_SPRITE_DIALOG_H_
#ifndef _D2D_TEXT_DIALOG_H_
#define _D2D_TEXT_DIALOG_H_

#include <wx/wx.h>

namespace d2d
{

class FontSprite;

class TextDialog : public wxDialog
{
public:
	TextDialog(wxWindow* parent, FontSprite* font);
	virtual ~TextDialog();

private:
	FontSprite* m_font;

	wxTextCtrl* m_text_ctrl;

}; // TextDialog

}

#endif // _D2D_TEXT_DIALOG_H_
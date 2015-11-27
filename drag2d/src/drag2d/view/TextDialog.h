#ifndef _D2D_TEXT_DIALOG_H_
#define _D2D_TEXT_DIALOG_H_

#include <wx/wx.h>

namespace d2d
{

class FontBlankSprite;

class TextDialog : public wxDialog
{
public:
	TextDialog(wxWindow* parent, FontBlankSprite* font);

private:
	void OnClose(wxCloseEvent& event);

private:
	FontBlankSprite* m_font;

	wxTextCtrl* m_text_ctrl;

	DECLARE_EVENT_TABLE()

}; // TextDialog

}

#endif // _D2D_TEXT_DIALOG_H_
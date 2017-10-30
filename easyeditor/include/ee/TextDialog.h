#ifndef _EASYEDITOR_TEXT_DIALOG_H_
#define _EASYEDITOR_TEXT_DIALOG_H_

#include <wx/wx.h>

#include <memory>

namespace ee
{

class FontBlankSprite;

class TextDialog : public wxDialog
{
public:
	TextDialog(wxWindow* parent, const std::shared_ptr<FontBlankSprite>& font);

private:
	void OnClose(wxCloseEvent& event);

private:
	std::shared_ptr<FontBlankSprite> m_font;

	wxTextCtrl* m_text_ctrl;

	DECLARE_EVENT_TABLE()

}; // TextDialog

}

#endif // _EASYEDITOR_TEXT_DIALOG_H_
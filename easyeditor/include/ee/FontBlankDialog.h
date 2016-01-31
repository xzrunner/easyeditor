#ifndef _EASYEDITOR_FONT_BLANK_DIALOG_H_
#define _EASYEDITOR_FONT_BLANK_DIALOG_H_

#include <wx/wx.h>

namespace ee
{

class FontBlankSymbol;

class FontBlankDialog : public wxDialog
{
public:
	FontBlankDialog(wxWindow* parent, FontBlankSymbol* symbol);

	~FontBlankDialog();

private:
	void InitLayout();

	void StoreSymbol();

private:
	FontBlankSymbol* m_symbol;

	wxTextCtrl* m_name;

	wxTextCtrl* m_font;

	wxTextCtrl* m_color;

	wxTextCtrl* m_align;
	wxTextCtrl* m_size;

	wxTextCtrl *m_width, *m_height;

}; // FontBlankDialog

}

#endif // _EASYEDITOR_FONT_BLANK_DIALOG_H_
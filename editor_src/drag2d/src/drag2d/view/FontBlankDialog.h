#pragma once

#include <wx/wx.h>

namespace d2d
{
	class FontBlankSymbol;

	class FontBlankDialog : public wxDialog
	{
	public:
		FontBlankDialog(wxWindow* parent, FontBlankSymbol* symbol);

		~FontBlankDialog();

	private:
		void initLayout();

		void storeSymbol();

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


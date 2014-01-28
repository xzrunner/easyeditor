#pragma once

#include <drag2d.h>

namespace complex
{
	class Symbol;

	class EditDialog : public wxDialog
	{
	public:
		EditDialog(wxWindow* parent, Symbol* symbol);

	private:
		void initLayout();

		void onClose(wxCloseEvent& event);

	private:
		d2d::EditPanel* m_stage;

		Symbol* m_symbol;

		DECLARE_EVENT_TABLE()

	}; // EditDialog
}


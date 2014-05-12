#pragma once

#include <drag2d.h>

namespace escale9
{
	class Symbol;

	class EditDialog : public wxDialog
	{
	public:
		EditDialog(wxWindow* parent, Symbol* symbol);

	private:
		void initLayout();

		void storeSymbol();

		void onClose(wxCloseEvent& event);

	private:
		Symbol* m_symbol;

		d2d::EditPanel* m_editPanel;

		DECLARE_EVENT_TABLE()

	}; // EditDialog
}


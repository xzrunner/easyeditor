#pragma once

#include <drag2d.h>

namespace lib9patch
{
	class EditDialog : public wxDialog
	{
	public:
		EditDialog(wxWindow* parent, d2d::Patch9Symbol* symbol);

	private:
		void initLayout();

		void storeSymbol();

		void onClose(wxCloseEvent& event);

	private:
		d2d::Patch9Symbol* m_symbol;

		d2d::EditPanel* m_editPanel;

		DECLARE_EVENT_TABLE()

	}; // EditDialog
}


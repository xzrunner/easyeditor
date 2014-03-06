#pragma once

#include <drag2d.h>

namespace escale9
{
	class EditDialog : public wxDialog
	{
	public:
		EditDialog(wxWindow* parent, d2d::Scale9Symbol* symbol);

	private:
		void initLayout();

		void storeSymbol();

		void onClose(wxCloseEvent& event);

	private:
		d2d::Scale9Symbol* m_symbol;

		d2d::EditPanel* m_editPanel;

		DECLARE_EVENT_TABLE()

	}; // EditDialog
}


#pragma once

#include <drag2d.h>

namespace libcomplex
{
	class EditDialog : public wxDialog
	{
	public:
		EditDialog(wxWindow* parent, d2d::ComplexSymbol* symbol);

	private:
		void initLayout();

		void onClose(wxCloseEvent& event);

	private:
		d2d::EditPanel* m_stage;

		d2d::ComplexSymbol* m_symbol;

		DECLARE_EVENT_TABLE()

	}; // EditDialog
}


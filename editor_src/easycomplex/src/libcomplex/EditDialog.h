#pragma once

#include <drag2d.h>

namespace libcomplex
{
	class EditDialog : public wxDialog
	{
	public:
		EditDialog(wxWindow* parent, d2d::ComplexSymbol* symbol,
			d2d::PropertySettingPanel* propertyPanel);

	private:
		void initLayout();

		void onClose(wxCloseEvent& event);

	private:
		d2d::ComplexSymbol* m_symbol;

		d2d::PropertySettingPanel* m_propertyPanel;

		d2d::EditPanel* m_editPanel;

		DECLARE_EVENT_TABLE()

	}; // EditDialog
}


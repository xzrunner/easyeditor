#pragma once

#include <drag2d.h>

namespace ecomplex
{
	class Symbol;

	class EditDialog : public wxDialog
	{
	public:
		EditDialog(wxWindow* parent, Symbol* symbol);

	private:
		void initLayout();

		void onClose(wxCloseEvent& event);

		void loadSymbolInfo();

	private:
		d2d::LibraryPanel* m_library;
		d2d::EditPanel* m_stage;
		d2d::ViewlistPanel* m_viewlist;

		Symbol* m_symbol;

		DECLARE_EVENT_TABLE()

	}; // EditDialog
}


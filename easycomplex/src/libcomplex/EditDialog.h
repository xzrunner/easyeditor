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

	void onCloseEvent(wxCloseEvent& event);
	void onClose(bool force, int returncode);

	void loadSymbolInfo();

private:
	d2d::EditPanel* m_stage;
	d2d::ViewlistPanel* m_viewlist;

	Symbol* m_symbol;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}


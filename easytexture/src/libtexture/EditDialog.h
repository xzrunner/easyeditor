#ifndef _EASYTEXTURE_EDIT_DIALOG_H_
#define _EASYTEXTURE_EDIT_DIALOG_H_

#include <drag2d.h>

namespace etexture
{

class Symbol;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, Symbol* symbol);
	virtual ~EditDialog();

private:
	void InitLayout();

	void OnClose(wxCloseEvent& event);

private:
	Symbol* m_symbol;

	d2d::EditPanel* m_stage;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYTEXTURE_EDIT_DIALOG_H_
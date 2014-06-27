#ifndef _EASYMESH_EDIT_DIALOG_H_
#define _EASYMESH_EDIT_DIALOG_H_

#include <drag2d.h>

namespace emesh
{

class Symbol;
class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, Symbol* symbol);
	virtual ~EditDialog();

private:
	void initLayout();

	void onClose(wxCloseEvent& event);

private:
	d2d::EditPanel* m_stage;

	Symbol* m_symbol;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYMESH_EDIT_DIALOG_H_
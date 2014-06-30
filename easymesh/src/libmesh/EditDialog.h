#ifndef _EASYMESH_EDIT_DIALOG_H_
#define _EASYMESH_EDIT_DIALOG_H_

#include <drag2d.h>

namespace emesh
{

class Sprite;

class EditDialog : public wxDialog
{
public:
	EditDialog(wxWindow* parent, Sprite* sprite);
	virtual ~EditDialog();

private:
	void initLayout();

	void onClose(wxCloseEvent& event);

private:
	d2d::EditPanel* m_stage;

	Sprite* m_sprite;

	DECLARE_EVENT_TABLE()

}; // EditDialog

}

#endif // _EASYMESH_EDIT_DIALOG_H_
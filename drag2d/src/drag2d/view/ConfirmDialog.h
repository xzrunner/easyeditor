#ifndef _DRAG2D_CONFIRM_DIALOG_H_
#define _DRAG2D_CONFIRM_DIALOG_H_

#include "BaseDialog.h"

namespace d2d
{

class ConfirmDialog : public BaseDialog
{
public:
	ConfirmDialog(wxWindow* parent);

private:
	void OnBtnYes(wxCommandEvent& event);
	void OnBtnNo(wxCommandEvent& event);

	void OnCloseEvent(wxCloseEvent& event);
	void OnClose(bool force, int returncode);

private:
	DECLARE_EVENT_TABLE()

}; // ConfirmDialog

}

#endif // _DRAG2D_CONFIRM_DIALOG_H_
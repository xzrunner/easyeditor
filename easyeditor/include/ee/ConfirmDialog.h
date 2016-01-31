#ifndef _EASYEDITOR_CONFIRM_DIALOG_H_
#define _EASYEDITOR_CONFIRM_DIALOG_H_

#include "BaseDialog.h"

namespace ee
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

#endif // _EASYEDITOR_CONFIRM_DIALOG_H_
#pragma once

#include "BaseDialog.h"

namespace d2d
{

class ExitDlg : public BaseDialog
{
public:
	ExitDlg(wxWindow* parent);

private:
	void onCloseEvent(wxCloseEvent& event);
	void onClose(bool force, int returncode);

	DECLARE_EVENT_TABLE()

}; //ExitDlg

}
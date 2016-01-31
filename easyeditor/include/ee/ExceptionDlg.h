#ifndef _EASYEDITOR_EXCEPTION_DIALOG_H_
#define _EASYEDITOR_EXCEPTION_DIALOG_H_

#include "BaseDialog.h"

namespace ee
{

class Exception;

class ExceptionDlg : public BaseDialog
{
public:
	ExceptionDlg(wxWindow* parent, const Exception& exp);
	ExceptionDlg(wxWindow* parent, const std::string& msg);

private:
	void InitLayout(const char* msg);

}; // ExceptionDlg

}

#endif // _EASYEDITOR_EXCEPTION_DIALOG_H_
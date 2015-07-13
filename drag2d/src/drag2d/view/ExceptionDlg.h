#ifndef _DRAG2D_EXCEPTION_DIALOG_H_
#define _DRAG2D_EXCEPTION_DIALOG_H_

#include "BaseDialog.h"

namespace d2d
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

#endif // _DRAG2D_EXCEPTION_DIALOG_H_
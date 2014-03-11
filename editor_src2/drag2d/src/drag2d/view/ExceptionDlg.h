#pragma once

#include "BaseDialog.h"

namespace d2d
{
	class Exception;

	class ExceptionDlg : public BaseDialog
	{
	public:
		ExceptionDlg(wxWindow* parent, const Exception& exp);

	private:
		void initLayout(const char* msg);

	}; // ExceptionDlg
}
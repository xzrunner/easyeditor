#pragma once

#include <wx/wx.h>

namespace d2d
{
	class Exception;

	class ExceptionDlg : public wxDialog
	{
	public:
		ExceptionDlg(wxWindow* parent, const Exception& exp);

	private:
		void initLayout(const char* msg);

	}; // ExceptionDlg
}
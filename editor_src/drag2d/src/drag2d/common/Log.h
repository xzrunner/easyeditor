#pragma once

#include <wx/wx.h>

namespace d2d
{
	static void log(int info)
	{
		wxMessageBox(wxString::FromDouble(info));
	}

	static void log(const wxString& info)
	{
		wxMessageBox(info);
	}
}


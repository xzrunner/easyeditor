#ifndef _EASYEDITOR_LOG_H_
#define _EASYEDITOR_LOG_H_

#include <wx/msgdlg.h>
#include <wx/string.h>

namespace ee
{

static void Log(int info)
{
	wxMessageBox(wxString::FromDouble(info));
}

static void Log(const wxString& info)
{
	wxMessageBox(info);
}

}

#endif // _EASYEDITOR_LOG_H_
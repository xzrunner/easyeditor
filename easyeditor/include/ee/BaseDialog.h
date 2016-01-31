#ifndef _EASYEDITOR_BASE_DIALOG_H_
#define _EASYEDITOR_BASE_DIALOG_H_

#include <wx/wx.h>

namespace ee
{

class BaseDialog : public wxDialog
{
public:
	BaseDialog(wxWindow* parent, std::string title, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize);

private:
	void MoveToCenter();

	void OnCloseEvent(wxCloseEvent& event);
	void OnClose(bool force, int returncode);

private:
	DECLARE_EVENT_TABLE()

}; // BaseDialog

}

#endif // _EASYEDITOR_BASE_DIALOG_H_
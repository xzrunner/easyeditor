#ifndef _DRAG2D_BASE_DIALOG_H_
#define _DRAG2D_BASE_DIALOG_H_

#include <wx/wx.h>

namespace d2d
{

class BaseDialog : public wxDialog
{
public:
	BaseDialog(wxWindow* parent, wxString title, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize);

private:
	void MoveToCenter();

	void OnCloseEvent(wxCloseEvent& event);
	void OnClose(bool force, int returncode);

private:
	DECLARE_EVENT_TABLE()

}; // BaseDialog

}

#endif // _DRAG2D_BASE_DIALOG_H_
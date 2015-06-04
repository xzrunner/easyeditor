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
	void toCenter();

	void onCloseEvent(wxCloseEvent& event);
	void onClose(bool force, int returncode);

	DECLARE_EVENT_TABLE()

}; // BaseDialog

}

#endif // _DRAG2D_BASE_DIALOG_H_
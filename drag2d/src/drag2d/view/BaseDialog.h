#ifndef _DRAG2D_BASE_DIALOG_H_
#define _DRAG2D_BASE_DIALOG_H_

#include <wx/wx.h>

namespace d2d
{

class BaseDialog : public wxDialog
{
public:
	BaseDialog(wxWindow* parent, wxString title);

private:
	void toCenter();

}; // BaseDialog

}

#endif // _DRAG2D_BASE_DIALOG_H_
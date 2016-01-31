#ifndef _EASYEDITOR_SET_VALUE_DIALOG_H_
#define _EASYEDITOR_SET_VALUE_DIALOG_H_

#include <wx/wx.h>

namespace ee
{

class SetValueDialog : public wxDialog
{
public:
	SetValueDialog(wxWindow* parent, const std::string& title, const std::string& oldVal, const wxPoint& pos);

	std::string GetText() const;

private:
	void InitLayout(const std::string& old_val);

private:
	wxTextCtrl* m_text_ctrl;

}; // SetValueDialog

}

#endif // _EASYEDITOR_SET_VALUE_DIALOG_H_
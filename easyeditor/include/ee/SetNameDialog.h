#ifndef _EASYEDITOR_SET_NAME_DIALOG_H_
#define _EASYEDITOR_SET_NAME_DIALOG_H_

#include "BaseDialog.h"

namespace ee
{

class SetNameDialog : public ee::BaseDialog
{
public:
	SetNameDialog(wxWindow* parent, const std::string& name);

	std::string GetShapeName() const;

private:
	void InitLayout(const std::string& name);

private:
	wxTextCtrl* m_name;

}; // SetNameDialog

}

#endif // _EASYEDITOR_SET_NAME_DIALOG_H_
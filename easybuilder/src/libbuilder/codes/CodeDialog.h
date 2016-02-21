#ifndef _EASYBUILDER_CODE_DIALOG_H_
#define _EASYBUILDER_CODE_DIALOG_H_

#include <wx/dialog.h>
#include <wx/notebook.h>

namespace ebuilder
{

class CodeDialog : public wxDialog
{
public:
	CodeDialog(wxWindow* parent);

public:
	wxNotebook* notebook;

}; // CodeDialog

class CodePanel : public wxPanel
{
public:
	CodePanel(CodeDialog* dlg);

}; // CodePanel

}

#endif // _EASYBUILDER_CODE_DIALOG_H_
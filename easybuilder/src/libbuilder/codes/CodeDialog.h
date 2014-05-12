
#ifndef LIBBUILDER_CODE_DIALOG_H
#define LIBBUILDER_CODE_DIALOG_H

#include <drag2d.h>

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

#endif // LIBBUILDER_CODE_DIALOG_H
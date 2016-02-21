#include "CodeDialog.h"

#include <wx/sizer.h>

namespace ebuilder
{

CodeDialog::CodeDialog(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, wxT("Codes"), wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION)
	, notebook(NULL)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new CodePanel(this), 1, wxEXPAND);
	SetSizer(sizer);
}

//////////////////////////////////////////////////////////////////////////
// class CodePanel
//////////////////////////////////////////////////////////////////////////

CodePanel::CodePanel(CodeDialog* dlg)
	: wxPanel(dlg)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	dlg->notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_BOTTOM);
	sizer->Add(dlg->notebook, 1, wxEXPAND);
	SetSizer(sizer);
}

}
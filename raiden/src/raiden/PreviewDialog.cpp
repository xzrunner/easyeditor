#include "PreviewDialog.h"
#include "PreviewCanvas.h"

using namespace raiden;

PreviewDialog::PreviewDialog(wxWindow* parent)
 	: wxDialog(parent, wxID_ANY, wxT("Preview"), wxDefaultPosition, 
	wxSize(480+80*2, 800), wxCLOSE_BOX | wxCAPTION)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	ee::EditPanel* editPanel = new ee::EditPanel(this, this);
	editPanel->setEditOP(new ee::NullEditOP(editPanel));
	editPanel->setCanvas(new PreviewCanvas(editPanel));

	sizer->Add(editPanel, 1, wxEXPAND);
	SetSizer(sizer);
}
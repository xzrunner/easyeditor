#include "PreviewDialog.h"
#include "PreviewPanel.h"
#include "StagePanel.h"
#include "Context.h"

namespace emodeling
{

PreviewDialog::PreviewDialog()
	: wxDialog(Context::Instance()->stage, wxID_ANY, wxT("Preview"), 
	wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new PreviewPanel(this, this), 1, wxEXPAND);
	SetSizer(sizer);
}

}
#include "EditDialog.h"
#include "StagePanel.h"

#include <ee/ConfirmDialog.h>

#include <wx/sizer.h>

namespace libskeleton
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, wxGLContext* glctx, Sprite* spr)
	: wxDialog(parent, wxID_ANY, "Edit Skeleton", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
{
	InitLayout(glctx, spr);
}

void EditDialog::InitLayout(wxGLContext* glctx, Sprite* spr)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	m_stage = new StagePanel(this, this, glctx, spr);
	sizer->Add(m_stage, 1, wxEXPAND);
	SetSizer(sizer);
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	if (!m_stage->IsEditDirty()) {
		Destroy();
		return;
	}

	ee::ConfirmDialog dlg(this);
	int val = dlg.ShowModal();
	if (val == wxID_YES) 
	{
		// todo store to spr
		Destroy();
	} 
	else if (val == wxID_NO) 
	{
//		m_sym->LoadFromFile(m_sym->GetFilepath());
		Destroy();
	}
}

}
#include "EditDialog.h"
#include "StagePanel.h"
#include "Sprite.h"

#include <ee/ConfirmDialog.h>

#include <sprite2/SkeletonSymbol.h>

#include <wx/sizer.h>

namespace libskeleton
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, wxGLContext* glctx, 
	                   const std::shared_ptr<Sprite>& spr)
	: wxDialog(parent, wxID_ANY, "Edit Skeleton", wxDefaultPosition, 
	wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_spr(spr)
{
	InitLayout(glctx);
}

void EditDialog::InitLayout(wxGLContext* glctx)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	auto sym = std::dynamic_pointer_cast<s2::SkeletonSymbol>(m_spr->GetSymbol());
	m_stage = new StagePanel(this, this, glctx, *sym->GetSkeleton());
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
	if (val == wxID_YES) {
		m_spr->SetPose(s2::SkeletonPose(m_stage->GetSkeleton()));
	} else if (val == wxID_NO) {
		;
	}
	Destroy();
}

}
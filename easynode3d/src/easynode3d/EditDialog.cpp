#include "EditDialog.h"
#include "Symbol.h"
#include "Sprite.h"
#include "StagePanel.h"
#include "StageCanvas.h"
#include "SprArrangeOP.h"

#include <ee/EditedFileStack.h>
#include <ee/ConfirmDialog.h>
#include <ee/sprite_msg.h>

namespace enode3d
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, wxGLContext* glctx, const std::shared_ptr<Sprite>& spr)
	: wxDialog(parent, wxID_ANY, "Edit Complex", wxDefaultPosition,
		wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_stage(nullptr)
{
	auto filepath = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol())->GetFilepath();
	SetTitle(filepath);
	InitLayout(glctx, spr);

	ee::EditedFileStack::Instance()->Push(filepath);
}

EditDialog::~EditDialog()
{
	ee::EditedFileStack::Instance()->Pop();
}

void EditDialog::InitLayout(wxGLContext* glctx, const std::shared_ptr<Sprite>& spr)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_stage = new StagePanel(this, this, nullptr);
	m_stage->SetCanvas(new StageCanvas(m_stage, glctx));
	m_stage->SetEditOP(new SprArrangeOP(*m_stage));

	ee::InsertSpriteSJ::Instance()->Insert(spr);

	sizer->Add(m_stage, 1, wxEXPAND);
	SetSizer(sizer);
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	Destroy();
}

}
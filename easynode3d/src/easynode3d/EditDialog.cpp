#include "EditDialog.h"
#include "Symbol.h"
#include "Sprite.h"
#include "StagePanel.h"
#include "StageCanvas.h"
#include "NodeArrangeOP.h"
#include "NodeFactory.h"

#include <ee/EditedFileStack.h>
#include <ee/ConfirmDialog.h>
#include <ee/sprite_msg.h>

#include <sprite2/SymType.h>

namespace enode3d
{

BEGIN_EVENT_TABLE(EditDialog, wxDialog)
	EVT_CLOSE(EditDialog::OnCloseEvent)
END_EVENT_TABLE()

EditDialog::EditDialog(wxWindow* parent, wxGLContext* glctx, const std::shared_ptr<Sprite>& spr)
	: wxDialog(parent, wxID_ANY, "Edit Complex", wxDefaultPosition,
		wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
	, m_stage(nullptr)
	, m_canvas(nullptr)
	, m_spr(spr)
{
	auto filepath = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol())->GetFilepath();
	SetTitle(filepath);
	InitLayout(glctx);

	ee::EditedFileStack::Instance()->Push(filepath);
}

EditDialog::~EditDialog()
{
	ee::EditedFileStack::Instance()->Pop();

	m_canvas->GetCamera().Reset();

	assert(m_spr->GetSymbol()->Type() == s2::SYM_MODEL);
	auto model_spr = std::dynamic_pointer_cast<s2::ModelSprite>(m_spr);
	model_spr->SetPos3(m_node->GetPos());
	model_spr->SetOri3(m_node->GetAngle());
}

void EditDialog::InitLayout(wxGLContext* glctx)
{
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_stage = new StagePanel(this, this, nullptr);
	m_canvas = new StageCanvas(m_stage, glctx);
	m_stage->SetCanvas(m_canvas);
	m_stage->SetEditOP(new NodeArrangeOP(*m_stage));

	m_node = NodeFactory::Instance()->Create(m_spr->GetSymbol());
	assert(m_spr->GetSymbol()->Type() == s2::SYM_MODEL);
	auto model_spr = std::dynamic_pointer_cast<s2::ModelSprite>(m_spr);
	m_node->SetPos(model_spr->GetPos3());
	m_node->SetAngle(model_spr->GetOri3());
	m_stage->InsertNode(m_node);

	sizer->Add(m_stage, 1, wxEXPAND);
	SetSizer(sizer);
}

void EditDialog::OnCloseEvent(wxCloseEvent& event)
{
	Destroy();
}

}
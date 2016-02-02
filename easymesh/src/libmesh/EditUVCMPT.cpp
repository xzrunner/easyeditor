#include "EditUVCMPT.h"
#include "StagePanel.h"
#include "EditUVOP.h"
#include "EditShape.h"

#include <ee/panel_msg.h>

namespace emesh
{

static const float SPEED_X = 0;
static const float SPEED_Y = -0.05f;

EditUVCMPT::EditUVCMPT(wxWindow* parent, const std::string& name, 
					   StagePanel* stage)
	: ee::EditCMPT(parent, name, stage->GetStageImpl())
	, m_stage(stage)
{
	m_editop = new EditUVOP(stage);
}

wxSizer* EditUVCMPT::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	{
		wxButton* btnMove = new wxButton(this, wxID_ANY, wxT("Move"));
		Connect(btnMove->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(EditUVCMPT::onMove));
		sizer->Add(btnMove);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btnReset = new wxButton(this, wxID_ANY, wxT("Reset"));
		Connect(btnReset->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(EditUVCMPT::onReset));
		sizer->Add(btnReset);
	}
	return sizer;
}

void EditUVCMPT::onMove(wxCommandEvent& event)
{
	if (Shape* shape = m_stage->GetShape()) {
		m_stage->GetShape()->OffsetUV(0, SPEED_Y);
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
	}
}

void EditUVCMPT::onReset(wxCommandEvent& event)
{
	if (EditShape* shape = static_cast<EditShape*>(m_stage->GetShape())) {
		shape->Reset();
		ee::SetCanvasDirtySJ::Instance()->SetDirty();
		m_editop->Clear();
	}
}

}
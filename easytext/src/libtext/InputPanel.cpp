#include "InputPanel.h"
#include "Sprite.h"

namespace etext
{

InputPanel::InputPanel(wxWindow* parent, Sprite* spr,
					   d2d::EditPanelImpl* stage_impl)
	: wxPanel(parent, wxID_ANY)
	, m_stage_impl(stage_impl)
{
	spr->Retain();
	m_spr = spr;

	InitLayout();
}

InputPanel::~InputPanel()
{
	m_spr->Release();
}

void InputPanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	{
		m_text_ctrl = new wxTextCtrl(this, wxID_ANY, m_spr->GetText(), 
			wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
		sizer->Add(m_text_ctrl, 12, wxEXPAND);
	}
	{
		m_enter_btn = new wxButton(this, wxID_ANY, "Enter");
		Connect(m_enter_btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(InputPanel::OnEnterPress));
		sizer->Add(m_enter_btn, 1, wxEXPAND);
	}
	sizer->Layout();

	SetSizer(sizer);
}

void InputPanel::OnEnterPress(wxCommandEvent& event)
{
	m_spr->SetText(m_text_ctrl->GetValue().ToStdString());
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
}

}
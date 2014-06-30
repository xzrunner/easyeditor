#include "ToolbarPanel.h"
#include "CreateMeshCMPT.h"
#include "EditMeshCMPT.h"
#include "EditUVCMPT.h"
#include "StagePanel.h"
#include "Sprite.h"

namespace emesh
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage, 
						   bool full, Sprite* sprite)
	: d2d::ToolbarPanel(parent, stage)
	, m_sprite(sprite)
{
	if (full)
	{
		addChild(new CreateMeshCMPT(this, wxT("Create"), stage));
	}
	addChild(new EditMeshCMPT(this, wxT("Edit"), stage));
	addChild(new EditUVCMPT(this, wxT("UV"), stage));
	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(initChildrenLayout());
	sizer->AddSpacer(10);
	{
		wxSizer* speed_sizer = new wxBoxSizer(wxHORIZONTAL);
		speed_sizer->Add(new wxStaticText(this, wxID_ANY, "speed x"), 0, wxLEFT | wxRIGHT, 5);

		m_ctrl_xspeed = new wxTextCtrl(this, wxID_ANY);
		Connect(m_ctrl_xspeed->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(ToolbarPanel::OnSetSpeed));
		m_ctrl_xspeed->SetValue(wxString::FromDouble(m_sprite->GetSpeed().x));
		speed_sizer->Add(m_ctrl_xspeed, 0, wxLEFT | wxRIGHT, 5);
		sizer->Add(speed_sizer);
	}
	{
		wxSizer* speed_sizer = new wxBoxSizer(wxHORIZONTAL);
		speed_sizer->Add(new wxStaticText(this, wxID_ANY, "speed y"), 0, wxLEFT | wxRIGHT, 5);

		m_ctrl_yspeed = new wxTextCtrl(this, wxID_ANY);
		Connect(m_ctrl_yspeed->GetId(), wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(ToolbarPanel::OnSetSpeed));
		m_ctrl_yspeed->SetValue(wxString::FromDouble(m_sprite->GetSpeed().y));
		speed_sizer->Add(m_ctrl_yspeed, 0, wxLEFT | wxRIGHT, 5);
		sizer->Add(speed_sizer);
	}
	return sizer;
}

void ToolbarPanel::OnSetSpeed(wxCommandEvent& event)
{
	double x, y;
	m_ctrl_xspeed->GetValue().ToDouble(&x);
	m_ctrl_yspeed->GetValue().ToDouble(&y);
	m_sprite->SetSpeed(d2d::Vector(x, y));
}

}
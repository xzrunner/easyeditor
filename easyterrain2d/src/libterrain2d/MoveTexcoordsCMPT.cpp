#include "MoveTexcoordsCMPT.h"
#include "WaveVerticesOP.h"
#include "StagePanel.h"
#include "OceanMesh.h"
#include "config.h"

namespace eterrain2d
{

MoveTexcoordsCMPT::MoveTexcoordsCMPT(wxWindow* parent, const wxString& name, StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
{
	m_editOP = new WaveVerticesOP(stage);
}

wxSizer* MoveTexcoordsCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	// switch
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("¿ª¹Ø"));
		check->SetValue(true);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(MoveTexcoordsCMPT::OnOpenMoveChanged));
		sizer->Add(check, 0);
	}
	sizer->AddSpacer(20);
	// speed
	{
		{
			wxSizer* spd_sizer = new wxBoxSizer(wxHORIZONTAL);
			wxStaticText* text = new wxStaticText(this, wxID_ANY, wxT("speed x:"));
			spd_sizer->Add(text);
			m_speed_x = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS, -50, 50, UV_SPEED_X);
			Connect(m_speed_x->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(MoveTexcoordsCMPT::OnChangeUVSpeed));
			spd_sizer->Add(m_speed_x, 0, wxLEFT, 5);
			sizer->Add(spd_sizer);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* spd_sizer = new wxBoxSizer(wxHORIZONTAL);
			wxStaticText* text = new wxStaticText(this, wxID_ANY, wxT("speed y:"));
			spd_sizer->Add(text);
			m_speed_y = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS, -50, 50, UV_SPEED_Y);
			Connect(m_speed_y->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(MoveTexcoordsCMPT::OnChangeUVSpeed));
			spd_sizer->Add(m_speed_y, 0, wxLEFT, 5);
			sizer->Add(spd_sizer);
		}
	}
	sizer->AddSpacer(20);
	return sizer;
}

void MoveTexcoordsCMPT::OnOpenMoveChanged(wxCommandEvent& event)
{
	std::vector<OceanMesh*>& oceans = static_cast<StagePanel*>(m_editPanel)->GetOceans();
	for (int i = 0, n = oceans.size(); i < n; ++i) {
		oceans[i]->OpenUVMove(event.IsChecked());
	}
}

void MoveTexcoordsCMPT::OnChangeUVSpeed(wxSpinEvent& event)
{
	d2d::Vector speed(m_speed_x->GetValue() * 0.01f, m_speed_y->GetValue() * 0.01f);
	std::vector<OceanMesh*>& oceans = static_cast<StagePanel*>(m_editPanel)->GetOceans();
	for (int i = 0, n = oceans.size(); i < n; ++i) {
		oceans[i]->SetTexcoordsSpeed(speed);
	}
}

}
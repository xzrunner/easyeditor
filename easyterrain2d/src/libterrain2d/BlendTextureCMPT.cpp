#include "BlendTextureCMPT.h"
#include "WaveVerticesOP.h"
#include "StagePanel.h"
#include "OceanMesh.h"
#include "config.h"

namespace eterrain2d
{

BlendTextureCMPT::BlendTextureCMPT(wxWindow* parent, const wxString& name, StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
{
	m_editOP = new WaveVerticesOP(stage);
}

wxSizer* BlendTextureCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	// switch
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("¿ª¹Ø"));
		check->SetValue(true);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(BlendTextureCMPT::OnOpenBlendChanged));
		sizer->Add(check, 0);
	}
	sizer->AddSpacer(20);
	// speed
	{
		wxSizer* spd_sizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* text = new wxStaticText(this, wxID_ANY, wxT("speed:"));
		spd_sizer->Add(text);
		m_speed = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS, 0, 100, BLEND_SPEED);
		Connect(m_speed->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(BlendTextureCMPT::OnChangeSpeed));
		spd_sizer->Add(m_speed, 0, wxLEFT, 5);
		sizer->Add(spd_sizer);
	}
	sizer->AddSpacer(20);
	return sizer;
}

void BlendTextureCMPT::OnOpenBlendChanged(wxCommandEvent& event)
{
	std::vector<OceanMesh*>& oceans = static_cast<StagePanel*>(m_editPanel)->GetOceans();
	for (int i = 0, n = oceans.size(); i < n; ++i) {
		oceans[i]->OpenBlend(event.IsChecked());
	}
}

void BlendTextureCMPT::OnChangeSpeed(wxSpinEvent& event)
{
	float speed = m_speed->GetValue() * 0.01f;
	std::vector<OceanMesh*>& oceans = static_cast<StagePanel*>(m_editPanel)->GetOceans();
	for (int i = 0, n = oceans.size(); i < n; ++i) {
		oceans[i]->SetBlendSpeed(speed);
	}
}

}
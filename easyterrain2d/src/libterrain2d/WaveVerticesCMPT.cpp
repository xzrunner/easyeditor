#include "WaveVerticesCMPT.h"
#include "WaveVerticesOP.h"
#include "StagePanel.h"
#include "OceanMesh.h"
#include "config.h"

namespace eterrain2d
{

WaveVerticesCMPT::WaveVerticesCMPT(wxWindow* parent, const wxString& name, StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
{
	m_editOP = new WaveVerticesOP(stage);
}

wxSizer* WaveVerticesCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	// switch
	{
		wxCheckBox* check = new wxCheckBox(this, wxID_ANY, wxT("开关"));
		check->SetValue(true);
		Connect(check->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(WaveVerticesCMPT::OnOpenWaveChanged));
		sizer->Add(check, 0);
	}
	sizer->AddSpacer(20);
	// reflush
	{
		wxButton* btn = new wxButton(this, wxID_ANY, "刷新");
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(WaveVerticesCMPT::OnReflushPress));
		sizer->Add(btn);
	}
	sizer->AddSpacer(20);
	// grid size
	{
		{
			wxSizer* size_sizer = new wxBoxSizer(wxHORIZONTAL);
			wxStaticText* text = new wxStaticText(this, wxID_ANY, wxT("行:"));
			size_sizer->Add(text);
			m_row_spin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS, 1, 100, MESH_ROW);
			Connect(m_row_spin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(WaveVerticesCMPT::OnChangeGridSize));
			size_sizer->Add(m_row_spin, 0, wxLEFT, 5);
			sizer->Add(size_sizer);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* size_sizer = new wxBoxSizer(wxHORIZONTAL);
			wxStaticText* text = new wxStaticText(this, wxID_ANY, wxT("列:"));
			size_sizer->Add(text);
			m_col_spin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS, 1, 100, MESH_COL);
			Connect(m_col_spin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(WaveVerticesCMPT::OnChangeGridSize));
			size_sizer->Add(m_col_spin, 0, wxLEFT, 5);
			sizer->Add(size_sizer);
		}
	}
	sizer->AddSpacer(20);
	// wave params
	{
		{
			wxSizer* size_sizer = new wxBoxSizer(wxHORIZONTAL);
			wxStaticText* text = new wxStaticText(this, wxID_ANY, wxT("速度:"));
			size_sizer->Add(text);
			m_wave_speed = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS, 1, 100, WAVE_SPEED);
			Connect(m_wave_speed->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(WaveVerticesCMPT::OnChangeWaveParams));
			size_sizer->Add(m_wave_speed, 0, wxLEFT, 5);
			sizer->Add(size_sizer);
		}
		sizer->AddSpacer(5);
		{
			wxSizer* size_sizer = new wxBoxSizer(wxHORIZONTAL);
			wxStaticText* text = new wxStaticText(this, wxID_ANY, wxT("幅度:"));
			size_sizer->Add(text);
			m_wave_height = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS, 1, 100, WAVE_HEIGHT);
			Connect(m_wave_height->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(WaveVerticesCMPT::OnChangeWaveParams));
			size_sizer->Add(m_wave_height, 0, wxLEFT, 5);
			sizer->Add(size_sizer);
		}
	}
	sizer->AddSpacer(20);
	// display
	{
		wxCheckBox* tris_edge = new wxCheckBox(this, wxID_ANY, wxT("triangle edge"));
		tris_edge->SetValue(d2d::Settings::bDisplayTrisEdge);
		Connect(tris_edge->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, 
			wxCommandEventHandler(WaveVerticesCMPT::OnChangeDisplayTriangles));
		sizer->Add(tris_edge);
	}
	return sizer;
}

void WaveVerticesCMPT::OnOpenWaveChanged(wxCommandEvent& event)
{
	std::vector<OceanMesh*>& oceans = static_cast<StagePanel*>(m_editPanel)->GetOceans();
	for (int i = 0, n = oceans.size(); i < n; ++i) {
		oceans[i]->OpenWave(event.IsChecked());
	}
}

void WaveVerticesCMPT::OnReflushPress(wxCommandEvent& event)
{
	std::vector<OceanMesh*>& oceans = static_cast<StagePanel*>(m_editPanel)->GetOceans();
	for (int i = 0, n = oceans.size(); i < n; ++i) {
		oceans[i]->Build();
	}
}

void WaveVerticesCMPT::OnChangeGridSize(wxSpinEvent& event)
{
	std::vector<OceanMesh*>& oceans = static_cast<StagePanel*>(m_editPanel)->GetOceans();
	for (int i = 0, n = oceans.size(); i < n; ++i) {
		oceans[i]->SetSmallGridSize(m_row_spin->GetValue(), m_col_spin->GetValue());
	}
}

void WaveVerticesCMPT::OnChangeWaveParams(wxSpinEvent& event)
{
	std::vector<OceanMesh*>& oceans = static_cast<StagePanel*>(m_editPanel)->GetOceans();
	for (int i = 0, n = oceans.size(); i < n; ++i) {
		oceans[i]->SetWaveInfo(m_wave_speed->GetValue() * 0.1f, m_wave_height->GetValue());
	}
}

void WaveVerticesCMPT::OnChangeDisplayTriangles(wxCommandEvent& event)
{
	static_cast<WaveVerticesOP*>(m_editOP)->m_draw_tris = event.IsChecked();
	m_editPanel->Refresh();
}

}
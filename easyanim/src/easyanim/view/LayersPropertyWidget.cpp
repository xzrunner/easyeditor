#include "LayersPropertyWidget.h"
#include "LayersPanel.h"
#include "Utility.h"
#include <wx/stattext.h>
#include <wx/spinctrl.h>

namespace eanim
{

LayersPropertyWidget::LayersPropertyWidget(LayersPanel* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, FRAME_GRID_HEIGHT), wxBORDER_NONE)
{
	m_parent = parent;
	initLayout();

	Connect(m_btnAdd->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersPropertyWidget::onAddLayer));
	Connect(m_btnDel->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersPropertyWidget::onDelLayer));
//	Connect(m_btnPlay->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersPropertyWidget::onPlay));
}

int LayersPropertyWidget::getFPSVal() const
{
	return m_fpsSpin->GetValue();
}

void LayersPropertyWidget::initLayout()
{
	SetBackgroundColour(MEDIUM_GRAY);

	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	m_btnAdd = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(25, 25));
	sizer->Add(m_btnAdd, 0, wxLEFT | wxRIGHT, 5);
	m_btnDel = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(25, 25));
	sizer->Add(m_btnDel, 0, wxLEFT | wxRIGHT, 5);

// 	m_btnPlay = new wxButton(this, wxID_ANY, "Play", wxDefaultPosition, wxSize(30, 25));
// 	sizer->Add(m_btnPlay, 0, wxLEFT, 10);

 	wxStaticText* fpsText = new wxStaticText(this, wxID_ANY, wxT("fps:"));
 	sizer->Add(fpsText, 0, wxLEFT, 30);
	m_fpsSpin = new wxSpinCtrl(this, wxID_ANY, wxT("24"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 60, 24);
	sizer->Add(m_fpsSpin, 0, wxLEFT, 5);

	SetSizer(sizer);
}

void LayersPropertyWidget::onAddLayer(wxCommandEvent& event)
{
	m_parent->onAddLayer();
}

void LayersPropertyWidget::onDelLayer(wxCommandEvent& event)
{
	m_parent->onDelLayer();
}

//void LayersPropertyWidget::onPlay(wxCommandEvent& event)
//{
//	m_parent->onPlay();
//}

} // eanim
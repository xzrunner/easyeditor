#include "LayersPropertyWidget.h"
#include "LayersPanel.h"
#include "Utility.h"

#include "frame/Context.h"

namespace eanim
{

LayersPropertyWidget::LayersPropertyWidget(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, FRAME_GRID_HEIGHT), wxBORDER_NONE)
{
	initLayout();
}

void LayersPropertyWidget::setFPS(int fps)
{
	m_fps->SetValue(fps);
}

void LayersPropertyWidget::initLayout()
{
	SetBackgroundColour(MEDIUM_GRAY);

	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* btnAdd = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(25, 25));
	Connect(btnAdd->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersPropertyWidget::onAddLayer));
	sizer->Add(btnAdd, 0, wxLEFT | wxRIGHT, 5);

	wxButton* btnDel = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(25, 25));
	Connect(btnDel->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersPropertyWidget::onDelLayer));
	sizer->Add(btnDel, 0, wxLEFT | wxRIGHT, 5);

// 	wxButton* btnPlay = new wxButton(this, wxID_ANY, "Play", wxDefaultPosition, wxSize(30, 25));
//	Connect(btnPlay->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersPropertyWidget::onPlay));
// 	sizer->Add(btnPlay, 0, wxLEFT, 10);

 	wxStaticText* fpsText = new wxStaticText(this, wxID_ANY, wxT("fps:"));
 	sizer->Add(fpsText, 0, wxLEFT, 30);
	m_fps = new wxSpinCtrl(this, wxID_ANY, wxT("30"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 60, 30);
	Connect(m_fps->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(LayersPropertyWidget::onChangeFps));
	sizer->Add(m_fps, 0, wxLEFT, 5);

	SetSizer(sizer);
}

void LayersPropertyWidget::onAddLayer(wxCommandEvent& event)
{
	static_cast<LayersPanel*>(Context::Instance()->layersPanel)->insertLayer();
}

void LayersPropertyWidget::onDelLayer(wxCommandEvent& event)
{
	static_cast<LayersPanel*>(Context::Instance()->layersPanel)->removeLayer();
}

//void LayersPropertyWidget::onPlay(wxCommandEvent& event)
//{
//	m_parent->onPlay();
//}

void LayersPropertyWidget::onChangeFps(wxSpinEvent& event)
{
	Context::Instance()->fps = event.GetValue();
}
} // eanim
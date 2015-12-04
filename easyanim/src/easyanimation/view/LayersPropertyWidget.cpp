#include "LayersPropertyWidget.h"
#include "config.h"

#include "message/messages.h"

namespace eanim
{

static const int DEFAULT_FPS = 30;

LayersPropertyWidget::LayersPropertyWidget(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, FRAME_GRID_HEIGHT), wxBORDER_NONE)
{
	InitLayout();

	RegistSubject(SetFpsSJ::Instance());
	RegistSubject(GetFpsSJ::Instance());
}

void LayersPropertyWidget::OnNotify(int sj_id, void* ud)
{
	switch (sj_id) 
	{
	case MSG_SET_FPS:
		{
			int* fps = (int*)ud;
			m_fps->SetValue(*fps);
		}
		break;
	case MSG_GET_FPS:
		{
			int* fps = (int*)ud;
			*fps = m_fps->GetValue();
		}
		break;
	}
}

void LayersPropertyWidget::InitLayout()
{
	SetBackgroundColour(MEDIUM_GRAY);

	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* btnAdd = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(25, 25));
	Connect(btnAdd->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersPropertyWidget::OnAddLayer));
	sizer->Add(btnAdd, 0, wxLEFT | wxRIGHT, 5);

	wxButton* btnDel = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(25, 25));
	Connect(btnDel->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersPropertyWidget::OnDelLayer));
	sizer->Add(btnDel, 0, wxLEFT | wxRIGHT, 5);

// 	wxButton* btnPlay = new wxButton(this, wxID_ANY, "Play", wxDefaultPosition, wxSize(30, 25));
//	Connect(btnPlay->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LayersPropertyWidget::onPlay));
// 	sizer->Add(btnPlay, 0, wxLEFT, 10);

 	wxStaticText* fps_text = new wxStaticText(this, wxID_ANY, wxT("fps:"));
 	sizer->Add(fps_text, 0, wxLEFT);
	m_fps = new wxSpinCtrl(this, wxID_ANY, wxString::FromDouble(DEFAULT_FPS), 
		wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxTE_PROCESS_ENTER, 1, 60, DEFAULT_FPS);
	Connect(m_fps->GetId(), wxEVT_SPINCTRL, wxSpinEventHandler(LayersPropertyWidget::OnChangeFps));
	sizer->Add(m_fps, 0, wxLEFT, 5);

	SetSizer(sizer);
}

void LayersPropertyWidget::OnAddLayer(wxCommandEvent& event)
{
	InsertLayerSJ::Instance()->Insert();
}

void LayersPropertyWidget::OnDelLayer(wxCommandEvent& event)
{
	RemoveLayerSJ::Instance()->Remove();
}

//void LayersPropertyWidget::onPlay(wxCommandEvent& event)
//{
//	m_parent->onPlay();
//}

void LayersPropertyWidget::OnChangeFps(wxSpinEvent& event)
{
	int fps = event.GetValue();
	SetFpsSJ::Instance()->Set(fps);
}

} // eanim
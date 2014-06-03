#include "ArrangeSpriteCMPT.h"
#include "ArrangeSpriteOP.h"
#include "StagePanel.h"

namespace sg
{

ArrangeSpriteCMPT::ArrangeSpriteCMPT(wxWindow* parent, const wxString& name,
									 StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
{
	m_editOP = new ArrangeSpriteOP(stage);
}

wxSizer* ArrangeSpriteCMPT::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("level"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Up"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ArrangeSpriteCMPT::onUpLevelPress));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Down"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ArrangeSpriteCMPT::onDownLevelPress));
		sizer->Add(btn);
	}
	return sizer;
}

void ArrangeSpriteCMPT::onUpLevelPress(wxCommandEvent& event)
{
	static_cast<StagePanel*>(m_editPanel)->ChangeSelectedSpritesLevel(true);
}

void ArrangeSpriteCMPT::onDownLevelPress(wxCommandEvent& event)
{
	static_cast<StagePanel*>(m_editPanel)->ChangeSelectedSpritesLevel(false);	
}

}
#include "AutoCutCMPT.h"
#include "AutoCutOP.h"
#include "StagePanel.h"

#include <easyimage.h>

namespace eimage
{

AutoCutCMPT::AutoCutCMPT(wxWindow* parent, const wxString& name, 
						 StagePanel* stage)
	: d2d::AbstractEditCMPT(parent, name, stage)
	, m_stage(stage)
{
	m_editOP = new AutoCutOP(stage);
}

wxSizer* AutoCutCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxButton* btn = new wxButton(this, wxID_ANY, wxT("Create Edge"));
	Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
		wxCommandEventHandler(AutoCutCMPT::CreateSymbolEdge));
	sizer->Add(btn);
	
	return sizer;
}

void AutoCutCMPT::CreateSymbolEdge(wxCommandEvent& event)
{
	const d2d::ISprite* sprite = m_stage->getImage();
	const d2d::ImageSprite* img_sprite 
		= dynamic_cast<const d2d::ImageSprite*>(sprite);
	assert(img_sprite);
	const d2d::Image* img = img_sprite->getSymbol().getImage();

	AutoCutOP* op = static_cast<AutoCutOP*>(m_editOP);
	BoundaryExtraction be(*img);
	be.GetBoundary(op->m_bound);
}

}
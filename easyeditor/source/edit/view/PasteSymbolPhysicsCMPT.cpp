#include "PasteSymbolPhysicsCMPT.h"
#include "BodyFactory.h"
#include "PasteSymbolPhysicsOP.h"
#include "PasteSymbolPhysicsRandomOP.h"

namespace ee
{
 
PasteSymbolPhysicsCMPT::PasteSymbolPhysicsCMPT(wxWindow* parent, const std::string& name, wxWindow* stage_wnd, EditPanelImpl* stage, 
											   MultiSpritesImpl* sprites_impl, LibraryPanel* library,
											   PhysicsPanelImpl* physicsImpl, b2World* world, b2World* worldReverse,
											   float* pScale /*= NULL*/, PasteSymbolRandomWidget* randomWidget/* = NULL*/)
	: EditCMPT(parent, name, stage)
	, m_world(world)
	, m_worldReverse(worldReverse)
{
	if (!randomWidget)
		m_editop = std::make_shared<PasteSymbolPhysicsOP>(stage_wnd, stage, library, physicsImpl, pScale);
	else
		m_editop = std::make_shared<PasteSymbolPhysicsRandomOP>(stage_wnd, stage, library, physicsImpl, randomWidget);
}

wxSizer* PasteSymbolPhysicsCMPT::InitLayout()
{
	wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	{
		wxArrayString choices;
		choices.Add(wxT("向下"));
		choices.Add(wxT("向上"));
		m_gravityChoice = new wxRadioBox(this, wxID_ANY, wxT("重力方向"), wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
		Connect(m_gravityChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(PasteSymbolPhysicsCMPT::onChangeGravity));
		topSizer->Add(m_gravityChoice);
	}
	topSizer->AddSpacer(10);
	{
		wxCheckBox* staticCtrl = new wxCheckBox(this, wxID_ANY, wxT("固定"));
		staticCtrl->SetValue(false);
		Connect(staticCtrl->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PasteSymbolPhysicsCMPT::onChangeStaticType));
		topSizer->Add(staticCtrl);
	}
	return topSizer;
}

void PasteSymbolPhysicsCMPT::onChangeGravity(wxCommandEvent& event)
{
	if (m_gravityChoice->GetSelection() == 0)
		BodyFactory::setWorld(m_world);
	else
		BodyFactory::setWorld(m_worldReverse);
}

void PasteSymbolPhysicsCMPT::onChangeStaticType(wxCommandEvent& event)
{
	auto op = std::dynamic_pointer_cast<PasteSymbolPhysicsOP>(m_editop);
	op->setStatic(event.IsChecked());
}

}
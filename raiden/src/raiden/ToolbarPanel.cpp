#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "Context.h"
#include "SelectSpritesOP.h"

using namespace raiden;

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: d2d::ToolbarPanel(parent, Context::Instance()->stage)
{
	Context* context = Context::Instance();

	addChild(new d2d::UniversalCMPT(this, wxT("paste"), context->stage, 
		new d2d::PasteSymbolOP(context->stage, context->stage, context->library)));
	addChild(new d2d::PasteSpriteCMPT(this, wxT("batch"), context->stage, context->stage, context->property));
	addChild(new d2d::UniversalCMPT(this, wxT("arrange"), context->stage, 
		new d2d::ArrangeSpriteOP<SelectSpritesOP>(context->stage, context->stage, context->property)));

	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(10);
	sizer->Add(initSettingLayout());
	sizer->AddSpacer(20);
	sizer->Add(Context::Instance()->layers = new d2d::LayersMgrWidget(this, m_editPanel));
	sizer->AddSpacer(20);
	sizer->Add(initChildrenLayout());
	return sizer;
}

wxSizer* ToolbarPanel::initSettingLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Settings"));
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("speed: ")));

		m_speed = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
			wxSP_ARROW_KEYS, 5, 100, 20);
		Connect(m_speed->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSpeedChanged));
		onSpeedChanged(wxSpinEvent());
		sizer->Add(m_speed);

		topSizer->Add(sizer);
	}
	return topSizer;
}

void ToolbarPanel::onSpeedChanged(wxSpinEvent& event)
{
	Context::Instance()->speed = m_speed->GetValue();
	m_editPanel->Refresh();
}
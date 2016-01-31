#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "Context.h"
#include "SelectSpritesOP.h"
#include "RandomPasteCMPT.h"

using namespace formation;

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: ee::ToolbarPanel(parent, Context::Instance()->stage)
{
	Context* context = Context::Instance();

	addChild(new ee::UniversalCMPT(this, wxT("paste"), context->stage, 
		new ee::PasteSymbolOP(context->stage, context->stage, context->library)));
	addChild(new ee::PasteSpriteCMPT(this, wxT("batch"), context->stage, context->stage, context->property));
	addChild(new ee::UniversalCMPT(this, wxT("arrange"), context->stage, 
		new ee::ArrangeSpriteOP<SelectSpritesOP>(context->stage, context->stage, context->property)));
	addChild(m_randomPaste = new RandomPasteCMPT(this, wxT("random"), context->stage));

	SetSizer(initLayout());	
}

void ToolbarPanel::initFromContext()
{
	Context* context = Context::Instance();

	m_width->SetValue(context->width);
	m_height->SetValue(context->height);
	m_time->SetValue(context->time);

	m_editPanel->Refresh();
}

void ToolbarPanel::updateByLibrary()
{
	m_randomPaste->updateByLibrary();
}

wxSizer* ToolbarPanel::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Settings"));
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("width:")));

		m_width = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 
			wxSP_ARROW_KEYS, 480, 2000, 800);
		Connect(m_width->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_width);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("height:")));

		m_height = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 
			wxSP_ARROW_KEYS, 1000, 20000, 2000);
		Connect(m_height->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_height);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(20);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("time:")));

		m_time = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 8, 256, 32);
		Connect(m_time->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_time);

		topSizer->Add(sizer);
	}
	onParamsChanged(wxSpinEvent());
	topSizer->AddSpacer(20);
	topSizer->Add(initChildrenLayout());

	return topSizer;
}

void ToolbarPanel::onParamsChanged(wxSpinEvent& event)
{
	Context* context = Context::Instance();

	if (context->width != m_width->GetValue() ||
		context->height != m_height->GetValue())
	{
		const float sx = (float) m_width->GetValue() / context->width,
			sy = (float) m_height->GetValue() / context->height;
		context->stage->resetAllSprites(sx, sy);
	}

	context->width = m_width->GetValue();
	context->height = m_height->GetValue();
	context->time = m_time->GetValue();

	m_editPanel->Refresh();
}
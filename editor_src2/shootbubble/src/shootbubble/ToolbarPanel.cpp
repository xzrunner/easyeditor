#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "Context.h"

using namespace shootbubble;

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: d2d::ToolbarPanel(parent, Context::Instance()->stage)
{
	Context* context = Context::Instance();

	addChild(new d2d::UniversalCMPT(this, wxT("paste"), context->stage, 
		new d2d::PasteSymbolOP(context->stage, context->stage, context->library)));
	addChild(new d2d::PasteSpriteCMPT(this, wxT("batch"), context->stage, context->stage, context->property));
	addChild(new d2d::UniversalCMPT(this, wxT("arrange"), context->stage, 
		new d2d::ArrangeSpriteFixOP(context->stage, context->stage, context->property)));

	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Settings"));
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("row:")));

		m_row = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 2, 30, 20);
		Connect(m_row->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_row);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("col:")));

		m_col = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 2, 20, 10);
		Connect(m_col->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_col);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(20);
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("length:")));

		m_edge = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 16, 512, 64);
		Connect(m_edge->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onParamsChanged));
		sizer->Add(m_edge);

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

	bool reset = context->edge != m_edge->GetValue();

	context->row = m_row->GetValue();
	context->col = m_col->GetValue();
	context->edge = m_edge->GetValue();

	if (reset)
		static_cast<StagePanel*>(m_editPanel)->updateAllSpritesLocation();

	m_editPanel->Refresh();
}
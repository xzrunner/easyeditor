#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "BodyEditCmpt.h"
#include "FixtureEditCmpt.h"
#include "JointEditCmpt.h"
#include "Context.h"

namespace emodeling
{

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: ee::ToolbarPanel(parent, Context::Instance()->stage->GetStageImpl())
{
	Context* context = Context::Instance();

	AddChild(new BodyEditCmpt(this, "Body", context->stage, context->property));
	AddChild(new FixtureEditCmpt(this, "Fixture", context->stage, context->property));
	AddChild(new JointEditCmpt(this, "Joint", context->stage, context->property));

	SetSizer(InitLayout());	
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(InitChildrenLayout());
	return topSizer;
}

}
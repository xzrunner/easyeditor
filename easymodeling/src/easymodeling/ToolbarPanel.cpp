
#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "BodyEditCmpt.h"
#include "FixtureEditCmpt.h"
#include "JointEditCmpt.h"
#include "Context.h"

using namespace emodeling;

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: ee::ToolbarPanel(parent, Context::Instance()->stage->GetStageImpl())
{
	Context* context = Context::Instance();

	addChild(new BodyEditCmpt(this, wxT("Body"), context->stage, context->property));
	addChild(new FixtureEditCmpt(this, wxT("Fixture"), context->stage, context->property));
	addChild(new JointEditCmpt(this, wxT("Joint"), context->stage, context->property));

	SetSizer(InitLayout());	
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(InitChildrenLayout());
	return topSizer;
}

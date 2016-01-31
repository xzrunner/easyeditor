#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "SelectSpritesOP.h"
#include "Context.h"

namespace coceditor
{

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: ee::ToolbarPanel(parent, Context::Instance()->stage)
{
	Context* context = Context::Instance();

// 	addChild(new ee::UniversalCMPT(this, wxT("paste"), context->stage, 
// 		new ee::ArrangeSpriteOP<ee::SelectSpritesOP>(context->stage, context->stage)));

	addChild(new ee::UniversalCMPT(this, wxT("paste"), context->stage, 
		new ee::ArrangeSpriteOP<SelectSpritesOP>(context->stage, context->stage, context->property)));

	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(initChildrenLayout());
	return topSizer;
}
} // coceditor
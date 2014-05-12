#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "SelectSpritesOP.h"
#include "Context.h"

namespace coceditor
{

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: d2d::ToolbarPanel(parent, Context::Instance()->stage)
{
	Context* context = Context::Instance();

// 	addChild(new d2d::UniversalCMPT(this, wxT("paste"), context->stage, 
// 		new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(context->stage, context->stage)));

	addChild(new d2d::UniversalCMPT(this, wxT("paste"), context->stage, 
		new d2d::ArrangeSpriteOP<SelectSpritesOP>(context->stage, context->stage, context->property)));

	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(initChildrenLayout());
	return topSizer;
}
} // coceditor
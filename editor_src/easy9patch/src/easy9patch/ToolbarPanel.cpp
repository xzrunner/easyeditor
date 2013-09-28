#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "Context.h"
#include "ComposeOP.h"
#include "ResizeCMPT.h"

namespace e9patch
{

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: d2d::ToolbarPanel(parent, Context::Instance()->stage)
{
	Context* context = Context::Instance();

 	addChild(new d2d::UniversalCMPT(this, wxT("compose"), context->stage, new ComposeOP()));
	addChild(Context::Instance()->resizeCMPT = new ResizeCMPT(this, wxT("resize"), context->stage));

	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(initChildrenLayout());
	return topSizer;
}
} // e9patch
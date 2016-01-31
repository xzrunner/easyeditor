#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "SelectSpritesOP.h"
#include "Context.h"

namespace coceditor
{
namespace complex
{
	ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* editPanel)
		: ee::ToolbarPanel(parent, editPanel)
	{
		Context* context = Context::Instance();

		addChild(new ee::UniversalCMPT(this, wxT("paste"), editPanel, 
			new ee::ArrangeSpriteOP<SelectSpritesOP>(editPanel, editPanel)));

		SetSizer(initLayout());	
	}

	wxSizer* ToolbarPanel::initLayout()
	{
		wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
		topSizer->Add(initChildrenLayout());
		return topSizer;
	}
}
}
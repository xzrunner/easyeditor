#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "Context.h"
#include "SelectSpritesOP.h"

//#include <easycomplex.h>

namespace ecomplex
{
	ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* editPanel,
		d2d::PropertySettingPanel* property)
		: d2d::ToolbarPanel(parent, editPanel)
	{
		Context* context = Context::Instance();

		addChild(new d2d::UniversalCMPT(this, wxT("paste"), editPanel, 
			new d2d::ArrangeSpriteOP<SelectSpritesOP>(editPanel, editPanel, property)));

		SetSizer(initLayout());	
	}

	wxSizer* ToolbarPanel::initLayout()
	{
		wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
		topSizer->Add(initChildrenLayout());
		return topSizer;
	}
}
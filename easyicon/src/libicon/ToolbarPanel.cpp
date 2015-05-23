#include "ToolBarPanel.h"
#include "StagePanel.h"

#include "EditRectOP.h"

namespace eicon
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	// Rect
	{
		d2d::AbstractEditOP* op = new EditRectOP(stage);
		addChild(new d2d::UniversalCMPT(this, "¾ØÐÎ", stage, op));
		stage->setEditOP(op);
	}
}

wxSizer* ToolbarPanel::initLayout()
{
	return NULL;
}

}
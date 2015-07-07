#include "ToolBarPanel.h"
#include "StagePanel.h"

#include "EditRectOP.h"
#include "EditChangedRectOP.h"
#include "EditChangedSectorOP.h"

namespace eicon
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	// Rect
	{
		d2d::AbstractEditOP* op = new EditRectOP(stage);
		addChild(new d2d::UniversalCMPT(this, "矩形", stage, op));
		stage->SetEditOP(op);
	}
 	// Changed Rect
 	{
 		d2d::AbstractEditOP* op = new EditChangedRectOP(stage);
 		addChild(new d2d::UniversalCMPT(this, "可变的矩形", stage, op));
 		stage->SetEditOP(op);
 	}
	// Changed Sector
	{
		d2d::AbstractEditOP* op = new EditChangedSectorOP(stage);
		addChild(new d2d::UniversalCMPT(this, "可变的扇形", stage, op));
		stage->SetEditOP(op);
	}

	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(initChildrenLayout());
	return sizer;
}

}
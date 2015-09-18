#include "ToolBarPanel.h"
#include "StagePanel.h"

#include "EditRectOP.h"
#include "EditQuadOP.h"
#include "EditChangedRectOP.h"
#include "EditChangedSectorOP.h"

namespace eicon
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage->GetStageImpl())
{
	// Rect
	{
		d2d::AbstractEditOP* op = new EditRectOP(stage);
		addChild(new d2d::UniversalCMPT(this, "矩形", stage->GetStageImpl(), op));
		stage->SetEditOP(op);
	}
	// Quad
	{
		d2d::AbstractEditOP* op = new EditQuadOP(stage);
		addChild(new d2d::UniversalCMPT(this, "四边形", stage->GetStageImpl(), op));
		stage->SetEditOP(op);
	}
 	// Changed Rect
 	{
 		d2d::AbstractEditOP* op = new EditChangedRectOP(stage);
 		addChild(new d2d::UniversalCMPT(this, "可变的矩形", stage->GetStageImpl(), op));
 		stage->SetEditOP(op);
 	}
	// Changed Sector
	{
		d2d::AbstractEditOP* op = new EditChangedSectorOP(stage);
		addChild(new d2d::UniversalCMPT(this, "可变的扇形", stage->GetStageImpl(), op));
		stage->SetEditOP(op);
	}

	SetSizer(initLayout());	
}

void ToolbarPanel::ChangeIconType(IconType type)
{
	switch (type) 
	{
	case IT_RECT:
		setChoice(0);
		break;
	case IT_QUAD:
		setChoice(1);
		break;
	case IT_CHANGED_RECT:
		setChoice(2);
		break;
	case IT_CHANGED_SECTOR:
		setChoice(3);
		break;
	}
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(initChildrenLayout());
	return sizer;
}

}
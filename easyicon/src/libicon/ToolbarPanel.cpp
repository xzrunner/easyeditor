#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "EditRectOP.h"
#include "EditQuadOP.h"
#include "EditChangedRectOP.h"
#include "EditChangedSectorOP.h"

#include <ee/UniversalCMPT.h>

namespace eicon
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
	// Rect
	{
		ee::EditOP* op = new EditRectOP(stage);
		AddChild(new ee::UniversalCMPT(this, "矩形", stage->GetStageImpl(), op));
		stage->SetEditOP(op);
	}
	// Quad
	{
		ee::EditOP* op = new EditQuadOP(stage);
		AddChild(new ee::UniversalCMPT(this, "四边形", stage->GetStageImpl(), op));
		stage->SetEditOP(op);
	}
 	// Changed Rect
 	{
 		ee::EditOP* op = new EditChangedRectOP(stage);
 		AddChild(new ee::UniversalCMPT(this, "可变的矩形", stage->GetStageImpl(), op));
 		stage->SetEditOP(op);
 	}
	// Changed Sector
	{
		ee::EditOP* op = new EditChangedSectorOP(stage);
		AddChild(new ee::UniversalCMPT(this, "可变的扇形", stage->GetStageImpl(), op));
		stage->SetEditOP(op);
	}

	SetSizer(InitLayout());	
}

void ToolbarPanel::ChangeIconType(IconType type)
{
	switch (type) 
	{
	case IT_RECT:
		SetChoice(0);
		break;
	case IT_QUAD:
		SetChoice(1);
		break;
	case IT_CHANGED_RECT:
		SetChoice(2);
		break;
	case IT_CHANGED_SECTOR:
		SetChoice(3);
		break;
	}
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(InitChildrenLayout());
	return sizer;
}

}
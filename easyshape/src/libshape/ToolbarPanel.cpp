#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "ShapeType.h"
#include "PolygonShape.h"

#include "EditPointOP.h"
#include "EditRectOP.h"
#include "EditCircleOP.h"
#include "EditPolylineOP.h"
#include "DrawComplexPolygonOP.h"
#include "EditBezierOP.h"

#include "DrawLineCMPT.h"
#include "DrawPolygon2CMPT.h"

#include <easyimage.h>

#include <ee/OneFloatValueCMPT.h>
#include <ee/SelectShapesOP.h>
#include <ee/shape_msg.h>
#include <ee/ImageSymbol.h>
#include <ee/ImageData.h>
#include <ee/FetchAllVisitor.h>

namespace eshape
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, ee::PropertySettingPanel* property,
						   StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
	, m_stage_panel(stage)
{
	stage->SetToolbarPanel(this);
	// point
	{
		ee::OneFloatValueCMPT* capture_cmpt = new ee::OneFloatValueCMPT(this, "点", stage->GetStageImpl(), "node capture", 5, 30, 10);
		capture_cmpt->SetEditOP(std::make_shared<EditPointOP>(stage, stage->GetStageImpl(), stage, capture_cmpt));
		AddChild(capture_cmpt);
	}
	// rect
	{
		ee::OneFloatValueCMPT* capture_cmpt = new ee::OneFloatValueCMPT(this, "矩形", stage->GetStageImpl(), "node capture", 5, 30, 10);
		capture_cmpt->SetEditOP(std::make_shared<EditRectOP>(stage, stage->GetStageImpl(), stage, property, capture_cmpt));
		AddChild(capture_cmpt);
	}
	// circle
	{
		ee::OneFloatValueCMPT* capture_cmpt = new ee::OneFloatValueCMPT(this, "圆形", stage->GetStageImpl(), "node capture", 5, 30, 10);
		capture_cmpt->SetEditOP(std::make_shared<EditCircleOP>(stage, stage->GetStageImpl(), stage, property, capture_cmpt));
		AddChild(capture_cmpt);
	}
	// chain
	AddChild(new DrawLineCMPT(this, "折线", stage, stage->GetStageImpl(), stage, property));
	// polygon
	AddChild(new DrawPolygon2CMPT(this, "多边形", stage, stage->GetStageImpl(), stage, property));
//	// complex polygon
//	{
//		ee::OneFloatValueCMPT* capture_cmpt = new ee::OneFloatValueCMPT(this, "复杂多边形", stage->GetStageImpl(), "node capture", 5, 30, 10);
////		ee::EditOP* op = new DrawComplexPolygonOP(stage, stage, property, capture_cmpt);
//
//		ee::EditOP* op = new EditPolylineOP<DrawComplexPolygonOP, ee::SelectShapesOP>
//			(stage, stage->GetStageImpl(), stage, property, capture_cmpt, capture_cmpt);
//
//		capture_cmpt->SetEditOP(op);
//		AddChild(capture_cmpt);
//	}
	// bezier
	{
		ee::OneFloatValueCMPT* capture_cmpt = new ee::OneFloatValueCMPT(this, "贝塞尔曲线", stage->GetStageImpl(), "node capture", 5, 30, 10);
		capture_cmpt->SetEditOP(std::make_shared<EditBezierOP>(stage, stage->GetStageImpl(), stage, property, capture_cmpt));
		AddChild(capture_cmpt);
	}
	SetSizer(InitLayout());	
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	sizer->Add(InitChildrenLayout());
	sizer->AddSpacer(20);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Clear"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnClearShapes));
		sizer->Add(btn);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Bounding"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::OnCreateBounding));
		sizer->Add(btn);
	}
	return sizer;
}

void ToolbarPanel::OnClearShapes(wxCommandEvent& event)
{
	ee::ClearShapeSJ::Instance()->Clear();
}

void ToolbarPanel::OnCreateBounding(wxCommandEvent& event)
{
	auto& bg = dynamic_cast<const Symbol&>(m_stage_panel->GetSymbol()).GetBG();
	auto img_symbol = std::dynamic_pointer_cast<ee::ImageSymbol>(bg);
	if (!img_symbol) {
		return;
	}

	ee::ClearShapeSJ::Instance()->Clear();

	auto img_data = ee::ImageDataMgr::Instance()->GetItem(img_symbol->GetFilepath());
	pimg::OutlineRaw raw(img_data->GetPixelData(), img_data->GetWidth(), img_data->GetHeight());
	raw.CreateBorderLineAndMerge();
	pimg::OutlineFine fine(raw.GetBorderLine(), raw.GetBorderLineMerged());
	fine.Trigger(0.04f, 0.2f);

	sm::vec2 offset(-img_data->GetWidth()*0.5f, -img_data->GetHeight()*0.5f);
	auto bounding = fine.GetResult();
	CU_VEC<sm::vec2> bounding2;
	for (int i = 0, n = bounding.size(); i < n; ++i) {
		bounding[i] += offset;
		bounding2.push_back(bounding[i]);
	}
	auto poly = std::make_unique<PolygonShape>(bounding2);
	ee::InsertShapeSJ::Instance()->Insert(std::move(poly));

	SetChoice(3);
}

void ToolbarPanel::SelectSuitableEditOP()
{
	std::vector<ee::ShapePtr> shapes;
	m_stage_panel->TraverseShapes(ee::FetchAllRefVisitor<ee::Shape>(shapes));
	if (shapes.empty()) return;

	ShapeType type = get_shape_type(shapes[0]->GetShapeDesc());
	switch (type)
	{
	case ST_CIRCLE:
		SetChoice(1);
		break;
	case ST_RECT:
		SetChoice(0); 
		break;
	case ST_BEZIER:
		SetChoice(4);
		break;
	case ST_POLYGON:
		SetChoice(3); 
		break;
	case ST_CHAIN:
		SetChoice(2); 
		break;
	}
}

}
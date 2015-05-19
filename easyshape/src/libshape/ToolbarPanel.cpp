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

namespace libshape
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::PropertySettingPanel* property,
						   StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	stage->SetToolbarPanel(this);
	// point
	{
		d2d::OneFloatValueCMPT* capture_cmpt = new d2d::OneFloatValueCMPT(this, "点", stage, "node capture", 5, 30, 10);
		d2d::AbstractEditOP* op = new EditPointOP(stage, stage, capture_cmpt);
		capture_cmpt->SetEditOP(op);
		addChild(capture_cmpt);
	}
	// rect
	{
		d2d::OneFloatValueCMPT* capture_cmpt = new d2d::OneFloatValueCMPT(this, "矩形", stage, "node capture", 5, 30, 10);
		d2d::AbstractEditOP* op = new EditRectOP(stage, stage, property, capture_cmpt);
		capture_cmpt->SetEditOP(op);
		addChild(capture_cmpt);
	}
	// circle
	{
		d2d::OneFloatValueCMPT* capture_cmpt = new d2d::OneFloatValueCMPT(this, "圆形", stage, "node capture", 5, 30, 10);
		d2d::AbstractEditOP* op = new EditCircleOP(stage, stage, property, capture_cmpt);
		capture_cmpt->SetEditOP(op);
		addChild(capture_cmpt);
	}
	// chain
	addChild(new DrawLineCMPT(this, wxT("折线"), stage, stage, property));
	// polygon
	addChild(new DrawPolygon2CMPT(this, "多边形", stage, stage, property));
	// complex polygon
	{
		d2d::OneFloatValueCMPT* capture_cmpt = new d2d::OneFloatValueCMPT(this, "复杂多边形", stage, "node capture", 5, 30, 10);
//		d2d::AbstractEditOP* op = new DrawComplexPolygonOP(stage, stage, property, capture_cmpt);

		d2d::AbstractEditOP* op = new EditPolylineOP<DrawComplexPolygonOP, d2d::SelectShapesOP>
			(stage, stage, property, capture_cmpt, capture_cmpt);

		capture_cmpt->SetEditOP(op);
		addChild(capture_cmpt);
	}
	// bezier
	{
		d2d::OneFloatValueCMPT* capture_cmpt = new d2d::OneFloatValueCMPT(this, "贝塞尔曲线", stage, "node capture", 5, 30, 10);
		d2d::AbstractEditOP* op = new EditBezierOP(stage, stage, property, capture_cmpt);
		capture_cmpt->SetEditOP(op);
		addChild(capture_cmpt);
	}
	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	sizer->Add(initChildrenLayout());
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
	static_cast<StagePanel*>(m_editPanel)->clearShapes();
	m_editPanel->Refresh();
}

void ToolbarPanel::OnCreateBounding(wxCommandEvent& event)
{
	StagePanel* stage = static_cast<StagePanel*>(m_editPanel);
	const d2d::ISymbol* bg = static_cast<const libshape::Symbol&>(stage->GetSymbol()).GetBG();
	const d2d::ImageSymbol* img_symbol = dynamic_cast<const d2d::ImageSymbol*>(bg);
	if (!img_symbol) {
		return;
	}

	stage->clearShapes();

	d2d::Image* img = img_symbol->getImage();
	eimage::ExtractOutlineRaw raw(*img);
	raw.CreateBorderLineAndMerge();
	eimage::ExtractOutlineFine fine(raw.GetBorderLine(), raw.GetBorderLineMerged());
	fine.Trigger(0.04f, 0.2f);

	d2d::Vector offset(-img->clipWidth()*0.5f, -img->clipHeight()*0.5f);
	std::vector<d2d::Vector> bounding = fine.GetResult();
	for (int i = 0, n = bounding.size(); i < n; ++i) {
		bounding[i] += offset;
	}
	stage->insertShape(new libshape::PolygonShape(bounding));

	setChoice(3);

	stage->Refresh();
}

void ToolbarPanel::SelectSuitableEditOP()
{
	std::vector<d2d::IShape*> shapes;
	static_cast<StagePanel*>(m_editPanel)->traverseShapes(
		d2d::FetchAllVisitor<d2d::IShape>(shapes));
	if (shapes.empty()) return;

	ShapeType type = get_shape_type(shapes[0]->GetShapeDesc());
	switch (type)
	{
	case ST_CIRCLE:
		setChoice(1);
		break;
	case ST_RECT:
		setChoice(0); 
		break;
	case ST_BEZIER:
		setChoice(4);
		break;
	case ST_POLYGON:
		setChoice(3); 
		break;
	case ST_CHAIN:
		setChoice(2); 
		break;
	}
}

}
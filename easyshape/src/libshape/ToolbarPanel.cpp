#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "Symbol.h"

#include "BezierShape.h"
#include "ChainShape.h"
#include "CircleShape.h"
#include "CosineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

#include "EditRectOP.h"
#include "EditCircleOP.h"
#include "EditPolylineOP.h"
#include "DrawPolygonEdgeOP.h"
#include "EditBezierOP.h"

#include "DrawLineCMPT.h"
#include "NodeCaptureCMPT.h"

#include <easyimage.h>

namespace libshape
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::PropertySettingPanel* property,
						   StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	stage->SetToolbarPanel(this);
	// rect
	{
		NodeCaptureCMPT* capture_cmpt = new NodeCaptureCMPT(this, "rect", stage);
		d2d::AbstractEditOP* op = new EditRectOP(stage, stage, property, capture_cmpt);
		capture_cmpt->SetEditOP(op);
		addChild(capture_cmpt);
	}
	// circle
	{
		NodeCaptureCMPT* capture_cmpt = new NodeCaptureCMPT(this, "circle", stage);
		d2d::AbstractEditOP* op = new EditCircleOP(stage, stage, property, capture_cmpt);
		capture_cmpt->SetEditOP(op);
		addChild(capture_cmpt);
	}
	// chain
	addChild(new DrawLineCMPT(this, wxT("chain"), stage, stage, property));
	// polygon
	{
		NodeCaptureCMPT* capture_cmpt = new NodeCaptureCMPT(this, "polygon", stage);
		d2d::AbstractEditOP* op = new EditPolylineOP<DrawPolygonEdgeOP, d2d::SelectShapesOP>
			(stage, stage, property, capture_cmpt, capture_cmpt);
		capture_cmpt->SetEditOP(op);
		addChild(capture_cmpt);
	}
	// bezier
	{
		NodeCaptureCMPT* capture_cmpt = new NodeCaptureCMPT(this, "bezier", stage);
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

	if (dynamic_cast<CircleShape*>(shapes[0]))
		setChoice(1);
	else if (dynamic_cast<RectShape*>(shapes[0]))
		setChoice(0); 
	else if (dynamic_cast<BezierShape*>(shapes[0]))
		setChoice(4);
	else if (dynamic_cast<PolygonShape*>(shapes[0]))
		setChoice(3); 
	else if (dynamic_cast<ChainShape*>(shapes[0]))
		setChoice(2); 
}

}
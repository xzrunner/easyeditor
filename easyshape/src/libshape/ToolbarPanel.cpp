#include "ToolbarPanel.h"
#include "StagePanel.h"

#include "BezierShape.h"
#include "ChainShape.h"
#include "CircleShape.h"
#include "CosineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

#include "EditRectOP.h"
#include "EditCircleOP.h"
#include "EditBezierOP.h"

#include "DrawLineCMPT.h"
#include "EditPolygonCMPT.h"

namespace libshape
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::PropertySettingPanel* property,
						   StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	stage->SetToolbarPanel(this);

	addChild(new NodeCaptureCMPT<EditRectOP>(this, wxT("rect"), stage, stage, property));
	addChild(new NodeCaptureCMPT<EditCircleOP>(this, wxT("circle"), stage, stage, property));
	addChild(new DrawLineCMPT(this, wxT("chain"), stage, stage, property));
	addChild(new EditPolygonCMPT(this, wxT("polygon"), stage, stage, property));
	addChild(new NodeCaptureCMPT<EditBezierOP>(this, wxT("bezier"), stage, stage, property));

	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(20);
	sizer->Add(initChildrenLayout());
	sizer->AddSpacer(20);
	{
		wxButton* btnClear = new wxButton(this, wxID_ANY, wxT("Clear"));
		Connect(btnClear->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(ToolbarPanel::onClearShapes));
		sizer->Add(btnClear);

	}
	return sizer;
}

void ToolbarPanel::onClearShapes(wxCommandEvent& event)
{
	static_cast<StagePanel*>(m_editPanel)->clearShapes();
	m_editPanel->Refresh();
}

void ToolbarPanel::selectSuitableEditOP()
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
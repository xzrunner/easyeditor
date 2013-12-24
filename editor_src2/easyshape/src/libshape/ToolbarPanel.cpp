
#include "ToolbarPanel.h"
#include "StagePanel.h"

using namespace libshape;

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::PropertySettingPanel* property,
						   StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	addChild(new d2d::NodeCaptureCMPT<d2d::EditRectOP>(this, wxT("rect"), stage, stage, property));
	addChild(new d2d::NodeCaptureCMPT<d2d::EditCircleOP>(this, wxT("circle"), stage, stage, property));
	addChild(new d2d::DrawLineCMPT(this, wxT("chain"), stage, stage, property));
	addChild(new d2d::EditPolygonCMPT(this, wxT("polygon"), stage, stage, property));
	addChild(new d2d::NodeCaptureCMPT<d2d::EditBezierOP>(this, wxT("bezier"), stage, stage, property));

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

	if (dynamic_cast<d2d::CircleShape*>(shapes[0]))
		setChoice(1);
	else if (dynamic_cast<d2d::RectShape*>(shapes[0]))
		setChoice(0); 
	else if (dynamic_cast<d2d::BezierShape*>(shapes[0]))
		setChoice(4);
	else if (dynamic_cast<d2d::PolygonShape*>(shapes[0]))
		setChoice(3); 
	else if (dynamic_cast<d2d::ChainShape*>(shapes[0]))
		setChoice(2); 
}
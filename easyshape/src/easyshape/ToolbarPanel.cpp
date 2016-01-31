#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "LibraryItem.h"

#include <easyshape.h>

namespace eshape
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, ee::PropertySettingPanel* property,
						   StagePanel* stage)
	: ee::ToolbarPanel(parent, stage)
{
	addChild(new libshape::NodeCaptureCMPT<libshape::EditRectOP>(this, wxT("rect"), stage, stage, property));
	addChild(new libshape::NodeCaptureCMPT<libshape::EditCircleOP>(this, wxT("circle"), stage, stage, property));
	addChild(new libshape::DrawLineCMPT(this, wxT("chain"), stage, stage, property));
	addChild(new libshape::EditPolygonCMPT(this, wxT("polygon"), stage, stage, property));
	addChild(new libshape::NodeCaptureCMPT<libshape::EditBezierOP>(this, wxT("bezier"), stage, stage, property));

	SetSizer(initLayout());	
}

void ToolbarPanel::changeCurrItem(LibraryItem* item)
{
	if (!item) return;

	std::vector<ee::IShape*>* shapes 
		= static_cast<std::vector<ee::IShape*>*>(item->getUserData());
	if (!shapes || shapes->empty()) return;

	ee::IShape* shape = (*shapes)[0];
	if (dynamic_cast<libshape::RectShape*>(shape))
		setChoice(0); 
	else if (dynamic_cast<libshape::CircleShape*>(shape))
		setChoice(1);
	else if (dynamic_cast<libshape::BezierShape*>(shape))
		setChoice(4);
	else if (dynamic_cast<libshape::PolygonShape*>(shape))
		setChoice(3); 
	else if (dynamic_cast<libshape::ChainShape*>(shape))
		setChoice(2); 
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

}
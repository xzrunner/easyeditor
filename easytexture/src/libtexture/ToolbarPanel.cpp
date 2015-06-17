#include "ToolBarPanel.h"
#include "StagePanel.h"

#include <easyshape.h>

namespace etexture
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	addChild(new libshape::DrawPolygonCMPT(this, wxT("polygon"), stage, stage, NULL, NULL));
	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
//	sizer->AddSpacer(20);
	sizer->Add(initChildrenLayout());
	return sizer;
}

}
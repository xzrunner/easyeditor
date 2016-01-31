#include "ToolBarPanel.h"
#include "StagePanel.h"

#include <easyshape.h>

namespace etexture
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
	addChild(new libshape::DrawPolygonCMPT(this, wxT("polygon"), stage, stage->GetStageImpl(), stage, NULL));
	SetSizer(InitLayout());	
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
//	sizer->AddSpacer(20);
	sizer->Add(InitChildrenLayout());
	return sizer;
}

}
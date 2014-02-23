#include "ToolbarPanel.h"
#include "CreateMeshCMPT.h"
#include "EditMeshCMPT.h"
#include "StagePanel.h"

namespace emesh
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	addChild(new CreateMeshCMPT(this, wxT("Create"), stage));
	addChild(new EditMeshCMPT(this, wxT("Edit"), stage));
	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(initChildrenLayout());
	return sizer;
}

}
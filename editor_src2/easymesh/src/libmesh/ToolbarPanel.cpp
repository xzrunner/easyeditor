#include "ToolbarPanel.h"
#include "CreateMeshCMPT.h"
#include "EditMeshCMPT.h"
#include "EditUVCMPT.h"
#include "StagePanel.h"

namespace emesh
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage, bool full)
	: d2d::ToolbarPanel(parent, stage)
{
	if (full)
	{
		addChild(new CreateMeshCMPT(this, wxT("Create"), stage));
	}
	addChild(new EditMeshCMPT(this, wxT("Edit"), stage));
	addChild(new EditUVCMPT(this, wxT("UV"), stage));
	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(initChildrenLayout());
	return sizer;
}

}
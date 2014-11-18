#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "RectCutCMPT.h"
#include "PixelDiffCMPT.h"
#include "AutoCutCMPT.h"

namespace eimage
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library, 
						   d2d::PropertySettingPanel* property, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	addChild(new RectCutCMPT(this, wxT("Rect Cut"), stage));
	addChild(new PixelDiffCMPT(this, wxT("Pixel Diff"), stage));
	addChild(new AutoCutCMPT(this, wxT("Pixel Diff"), stage));

	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	return initChildrenLayout();
}
	
}
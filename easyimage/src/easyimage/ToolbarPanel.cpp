#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "RectCutCMPT.h"
#include "PixelDiffCMPT.h"
#include "AutoTriCutCMPT.h"
#include "AutoRectCutCMPT.h"

#include <easyshape.h>

namespace eimage
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, d2d::LibraryPanel* library, 
						   d2d::PropertySettingPanel* property, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	addChild(new RectCutCMPT(this, wxT("Rect Cut"), stage));
	addChild(new AutoTriCutCMPT(this, wxT("Auto Tri Cut"), stage));
	addChild(new AutoRectCutCMPT(this, wxT("Auto Rect Cut"), stage));
	addChild(new PixelDiffCMPT(this, wxT("Pixel Diff"), stage));
	SetSizer(initLayout());	
}

wxSizer* ToolbarPanel::initLayout()
{
	return initChildrenLayout();
}
	
}
#include "ToolbarPanel.h"
#include "StagePanel.h"
#include "RectCutCMPT.h"
#include "PixelDiffCMPT.h"
#include "AutoTriCutCMPT.h"
#include "AutoRectCutCMPT.h"
#include "ImageProcessCMPT.h"

#include <easyshape.h>

namespace eimage
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, ee::LibraryPanel* library, 
						   StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
	addChild(new RectCutCMPT(this, wxT("Rect Cut"), stage));
	addChild(new AutoTriCutCMPT(this, wxT("Auto Tri Cut"), stage));
	addChild(new AutoRectCutCMPT(this, wxT("Auto Rect Cut"), stage));
	addChild(new PixelDiffCMPT(this, wxT("Pixel Diff"), stage));
	addChild(new ImageProcessCMPT(this, wxT("Image Process"), stage));
	SetSizer(InitLayout());	
}

wxSizer* ToolbarPanel::InitLayout()
{
	return InitChildrenLayout();
}
	
}
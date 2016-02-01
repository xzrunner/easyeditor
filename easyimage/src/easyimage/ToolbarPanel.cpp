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
	AddChild(new RectCutCMPT(this, "Rect Cut", stage));
	AddChild(new AutoTriCutCMPT(this, "Auto Tri Cut", stage));
	AddChild(new AutoRectCutCMPT(this, "Auto Rect Cut", stage));
	AddChild(new PixelDiffCMPT(this, "Pixel Diff", stage));
	AddChild(new ImageProcessCMPT(this, "Image Process", stage));
	SetSizer(InitLayout());	
}

wxSizer* ToolbarPanel::InitLayout()
{
	return InitChildrenLayout();
}
	
}
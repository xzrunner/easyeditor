#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "CreateRegionCMPT.h"
#include "WaveVerticesCMPT.h"
#include "MoveTexcoordsCMPT.h"
#include "BlendTextureCMPT.h"

namespace eterrain2d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: d2d::ToolbarPanel(parent, stage)
{
	addChild(new CreateRegionCMPT(this, wxT("创建区域"), stage));
	addChild(new WaveVerticesCMPT(this, wxT("顶点波动"), stage));
	addChild(new MoveTexcoordsCMPT(this, wxT("纹理移动"), stage));
	addChild(new BlendTextureCMPT(this, wxT("纹理混合"), stage));
	SetSizer(initLayout());
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(initChildrenLayout());
	return sizer;
}

}
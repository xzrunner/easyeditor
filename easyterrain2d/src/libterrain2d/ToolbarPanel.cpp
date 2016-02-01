#include "ToolBarPanel.h"
#include "StagePanel.h"
#include "CreateRegionCMPT.h"
#include "WaveVerticesCMPT.h"
#include "MoveTexcoordsCMPT.h"
#include "BlendTextureCMPT.h"

namespace eterrain2d
{

ToolbarPanel::ToolbarPanel(wxWindow* parent, StagePanel* stage)
	: ee::ToolbarPanel(parent, stage->GetStageImpl())
{
	AddChild(new CreateRegionCMPT(this, wxT("创建区域"), stage));
	AddChild(m_wave_cmpt = new WaveVerticesCMPT(this, wxT("顶点波动"), stage));
	AddChild(m_move_uv_cmpt = new MoveTexcoordsCMPT(this, wxT("纹理移动"), stage));
	AddChild(m_blend_tex_cmpt = new BlendTextureCMPT(this, wxT("纹理混合"), stage));
	SetSizer(InitLayout());
}

void ToolbarPanel::SetControlersValue(const OceanMesh* ocean)
{
	m_wave_cmpt->SetControlersValue(ocean);
	m_move_uv_cmpt->SetControlersValue(ocean);
	m_blend_tex_cmpt->SetControlersValue(ocean);
}

wxSizer* ToolbarPanel::InitLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(InitChildrenLayout());
	return sizer;
}

}
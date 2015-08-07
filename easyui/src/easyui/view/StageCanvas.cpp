#include "StageCanvas.h"
#include "StagePanel.h"

#include "frame/SettingCfg.h"
#include "dataset/TidCfg.h"

namespace eui
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	static bool inited = false;
	if (!inited) {
		m_stage->InitConfig();
		TidCfg::Instance()->LoadConfig();
		inited = true;
	}

	DrawGuideLines();

	d2d::Rect sr = m_screen.GetRegion();
	m_stage->TraverseSprites(d2d::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		d2d::DT_VISIBLE);

	m_stage->GetAnchorMgr()->Draw();

	m_stage->DrawEditOP();

#ifdef _DEBUG 
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->DebugDraw();
	}
#endif
}

void StageCanvas::DrawGuideLines() const
{
	SettingCfg* cfg = SettingCfg::Instance();

	d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 
		cfg->m_view_width * 0.5f,
		cfg->m_view_height * 0.5f,
		d2d::LIGHT_GREY_LINE);
}

}
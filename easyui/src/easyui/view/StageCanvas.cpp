#include "StageCanvas.h"
#include "StagePanel.h"

#include "frame/SettingCfg.h"
#include "dataset/TidCfg.h"

namespace eui
{

StageCanvas::StageCanvas(StagePanel* statge)
	: d2d::DynamicStageCanvas(statge)
	, m_statge(statge)
{
}

void StageCanvas::OnDraw()
{
	static bool inited = false;
	if (!inited) {
		m_statge->InitConfig();
		TidCfg::Instance()->LoadConfig();
		inited = true;
	}

	DrawGuideLines();

	d2d::Rect sr = m_screen.GetRegion();
	m_statge->traverseSprites(d2d::DrawSpritesVisitor(sr), d2d::DT_VISIBLE);

	m_statge->GetAnchorMgr()->Draw();

	m_stage->drawEditTemp();

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
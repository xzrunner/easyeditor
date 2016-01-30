#include "StageCanvas.h"
#include "StagePanel.h"
#include "TidCfg.h"
#include "QueryWindowViewSizeSJ.h"

namespace eui
{
namespace window
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage, stage->GetStageImpl(), NULL, false)
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
		d2d::DrawCallBatching::Instance()->DebugDraw();
	}
#endif
}

void StageCanvas::DrawGuideLines() const
{
	int width, height;
	window::QueryWindowViewSizeSJ::Instance()->Query(width, height);

	d2d::PrimitiveDraw::DrawRect(d2d::Vector(0, 0), 
		width * 0.5f,
		height * 0.5f,
		d2d::LIGHT_GREY_LINE);
}

}
}
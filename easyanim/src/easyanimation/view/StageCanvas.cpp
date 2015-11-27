#include "StageCanvas.h"
#include "StagePanel.h"

namespace eanim
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage, stage->GetStageImpl())
	, m_stage_panel(stage)
	, m_background(NULL)
{
}

StageCanvas::~StageCanvas()
{
	if (m_background) {
		m_background->Release();
	}
}

void StageCanvas::SetBackground(d2d::ISymbol* symbol)
{
	d2d::obj_assign<d2d::ISymbol>(m_background, symbol);
}

void StageCanvas::OnDrawSprites() const
{
	DrawBackground();
	std::vector<d2d::ISprite*> sprites;
	static_cast<StagePanel*>(m_stage_panel)->TraverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites), d2d::DT_VISIBLE);
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		d2d::SpriteRenderer::Instance()->Draw(sprites[i]);
	m_stage->DrawEditOP();

// #ifdef _DEBUG 
// 	if (d2d::Config::Instance()->IsUseDTex()) {
// 		d2d::DrawCallBatching::Instance()->DebugDraw();
// 	}
// #endif
}

void StageCanvas::DrawBackground() const
{
	if (m_background) {
		d2d::SpriteRenderer::Instance()->Draw(m_background);
	}

	float xedge = GetSize().GetWidth() * 0.5f;
	float yedge = GetSize().GetHeight() * 0.5f;
	d2d::PrimitiveDraw::cross(d2d::Vector(0,0), xedge, yedge, d2d::LIGHT_GREY);
}

} // eanim
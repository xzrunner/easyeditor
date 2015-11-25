#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

namespace emesh
{

StageCanvas::StageCanvas(StagePanel* panel)
	: d2d::OrthoCanvas(panel, panel->GetStageImpl())
	, m_panel(panel)
{
}

void StageCanvas::InitGL()
{
	d2d::OrthoCanvas::InitGL();

	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DrawCallBatching::Instance()->ReloadBegin();
	}
	m_panel->GetSymbol()->ReloadTexture();
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DrawCallBatching::Instance()->ReloadEnd();
	}
}

void StageCanvas::OnDrawSprites() const
{
	const d2d::ISprite* bg = m_panel->GetBackground();
	if (bg) {
		d2d::SpriteRenderer::Instance()->Draw(bg);
	}

	m_stage->DrawEditOP();

	m_panel->TraverseShapes(d2d::DrawShapesVisitor(d2d::Rect()), d2d::DT_VISIBLE);
}

}
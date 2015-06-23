#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

namespace emesh
{

StageCanvas::StageCanvas(StagePanel* panel)
	: d2d::DynamicStageCanvas(panel)
	, m_panel(panel)
{
}

void StageCanvas::initGL()
{
	d2d::DynamicStageCanvas::initGL();
	m_panel->GetSymbol()->reloadTexture();
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->ReloadTexture();
	}
	resetViewport();
}

void StageCanvas::OnDraw()
{
	const d2d::ISprite* bg = m_panel->GetBackground();
	if (bg) {
		d2d::SpriteRenderer::Instance()->Draw(bg);
	}

	m_stage->drawEditTemp();

	m_panel->traverseShapes(d2d::DrawShapesVisitor(d2d::Rect()), d2d::DT_VISIBLE);

#ifdef _DEBUG 
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->DebugDraw();
	}
#endif
}

}
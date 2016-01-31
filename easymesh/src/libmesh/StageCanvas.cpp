#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

namespace emesh
{

StageCanvas::StageCanvas(StagePanel* panel)
	: ee::OrthoCanvas(panel, panel->GetStageImpl())
	, m_panel(panel)
{
}

void StageCanvas::OnDrawSprites() const
{
	const ee::Sprite* bg = m_panel->GetBackground();
	if (bg) {
		ee::SpriteRenderer::Instance()->Draw(bg);
	}

	m_stage->DrawEditOP();

	m_panel->TraverseShapes(ee::DrawShapesVisitor(ee::Rect()), ee::DT_VISIBLE);
}

}
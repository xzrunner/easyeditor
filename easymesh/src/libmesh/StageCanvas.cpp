#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

#include <ee/SpriteRenderer.h>
#include <ee/EditPanelImpl.h>
#include <ee/DrawShapesVisitor.h>

namespace emesh
{

StageCanvas::StageCanvas(StagePanel* panel,
						 wxGLContext* glctx)
	: ee::CameraCanvas(panel, panel->GetStageImpl(), glctx)
	, m_panel(panel)
{
}

void StageCanvas::OnDrawSprites() const
{
	const ee::Sprite* bg = m_panel->GetBackground();
	if (bg) {
		ee::SpriteRenderer::Draw(bg);
	}

	m_stage->DrawEditOP();

	m_panel->TraverseShapes(ee::DrawShapesVisitor(ee::Rect()), ee::DT_VISIBLE);
}

}
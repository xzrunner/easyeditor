#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

#include <ee/SpriteRenderer.h>
#include <ee/EditPanelImpl.h>
#include <ee/DrawShapesVisitor.h>
#include <ee/Config.h>

namespace emesh
{

StageCanvas::StageCanvas(StagePanel* panel,
						 wxGLContext* glctx)
	: ee::CameraCanvas(panel, panel->GetStageImpl(), gum::CAM_ORTHO2D, glctx)
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

	m_panel->TraverseShapes(ee::DrawShapesVisitor(sm::rect(), GetCameraScale()), ee::DT_VISIBLE);
}

}
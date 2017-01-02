#include "StageCanvas.h"
#include "StagePanel.h"
#include "dataset/Mesh.h"

#include <ee/SpriteRenderer.h>
#include <ee/FetchAllVisitor.h>
#include <ee/Sprite.h>
#include <ee/DrawSpritesVisitor.h>
#include <ee/DrawShapesVisitor.h>

#include <sprite2/S2_RVG.h>

namespace enav
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::CameraCanvas(stage, stage->GetStageImpl(), gum::CAM_ORTHO2D)
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	s2::RVG::SetColor(s2::Color(204, 204, 204));
	s2::RVG::LineWidth(2);
	const sm::rect& r = m_stage->GetRegion();
	s2::RVG::Rect(sm::vec2(r.xmin, r.ymin), sm::vec2(r.xmax, r.ymax), false);

	m_stage->TraverseSprites(ee::DrawSpritesVisitor(r, 1), ee::DT_VISIBLE);
	m_stage->TraverseShapes(ee::DrawShapesVisitor(r, 1), ee::DT_VISIBLE);

	m_stage->GetMesh()->Draw();

	m_stage->DrawEditOP();
}

}
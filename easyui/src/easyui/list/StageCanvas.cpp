#include "StageCanvas.h"
#include "StagePanel.h"

namespace eui
{
namespace list
{

StageCanvas::StageCanvas(StagePanel* stage, wxGLContext* glctx)
	: d2d::OrthoCanvas(stage, stage->GetStageImpl(), glctx, false)
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	DrawGuideLines();

	d2d::Rect sr = m_screen.GetRegion();
	m_stage->TraverseSprites(d2d::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		d2d::DT_VISIBLE);

	m_stage->DrawEditOP();
}

void StageCanvas::DrawGuideLines() const
{
	d2d::PrimitiveDraw::Cross(d2d::Vector(0, 0), 100, 100, d2d::Colorf(1, 0, 0));
}

}
}
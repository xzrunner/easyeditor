#include "StageCanvas.h"
#include "StagePanel.h"

namespace ep3dinv
{

StageCanvas::StageCanvas(StagePanel* stage)
//	: d2d::DynamicStageCanvas(stage)
	: d2d::FBOCanvas(stage)
	, m_stage(stage)
{
}

void StageCanvas::OnSize(int w, int h)
{
	d2d::FBOCanvas::OnSize(w, h);

	//////////////////////////////////////////////////////////////////////////

	SetCurrentCanvas();

	glViewport(0, 0, w, h);

	m_screen.SetSize(w, h);
	m_screen.SetCamera();

	m_camera->UpdateModelView();

	d2d::ShaderMgr::Instance()->SetProjection(w, h);
}

void StageCanvas::OnDrawSprites() const
{
	m_stage->traverseSprites(d2d::DrawSpritesVisitor(), d2d::DT_VISIBLE);

	m_stage->drawEditTemp();
}

}
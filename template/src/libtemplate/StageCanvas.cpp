#include "StageCanvas.h"
#include "StagePanel.h"

#include <ee/Camera.h>
#include <ee/RenderContextStack.h>
#include <ee/DrawSpritesVisitor.h>

namespace etemplate
{

StageCanvas::StageCanvas(StagePanel* stage)
//	: ee::OrthoCanvas(stage)
	: ee::TwoPassCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnSize(int w, int h)
{
	ee::TwoPassCanvas::OnSize(w, h);

	//////////////////////////////////////////////////////////////////////////

	SetCurrentCanvas();

	glViewport(0, 0, w, h);

	m_screen.SetSize(w, h);
	m_screen.SetCamera();

	m_camera->UpdateModelView();

	ee::RenderContextStack::Instance()->SetProjection(w, h);
}

void StageCanvas::OnDrawSprites() const
{
	ee::Rect sr = m_screen.GetRegion();
	m_stage->TraverseSprites(ee::DrawSpritesVisitor(sr, m_camera->GetScale()), 
		ee::DT_VISIBLE);

	m_stage->DrawEditOP();
}

}
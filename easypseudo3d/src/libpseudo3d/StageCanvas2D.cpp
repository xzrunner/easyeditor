#include "StageCanvas2D.h"

#include "Projection2DScene.h"

namespace epseudo3d
{

StageCanvas2D::StageCanvas2D(wxWindow* stage_wnd, ee::EditPanelImpl* stage)
	: ee::OrthoCanvas(stage_wnd, stage)
{
	SetBgColor(ee::BLACK);

	m_scene = new Projection2DScene(stage);
}

void StageCanvas2D::OnDrawSprites() const
{
	m_scene->Draw();

	m_stage->DrawEditOP();
}

}
#include "StageCanvas2D.h"

#include "Projection2DScene.h"

namespace epseudo3d
{

StageCanvas2D::StageCanvas2D(d2d::EditPanel* stage)
	: d2d::OrthoCanvas(stage)
{
	SetBgColor(d2d::BLACK);

	m_scene = new Projection2DScene(stage);
}

void StageCanvas2D::InitGL()
{
	d2d::OrthoCanvas::InitGL();

	m_scene->Load(NULL);

	// for Camera reset when init
	Refresh();
}

void StageCanvas2D::OnDrawSprites() const
{
	m_scene->Draw();

	m_stage->getEditOP()->OnDraw();
}

}
#include "StageCanvas2D.h"

#include "Projection2DScene.h"

namespace epseudo3d
{

StageCanvas2D::StageCanvas2D(d2d::EditPanel* stage)
	: d2d::OrthoCanvas(stage)
{
	setBgColor(d2d::BLACK);

	m_scene = new Projection2DScene(stage);
}

void StageCanvas2D::initGL()
{
	d2d::OrthoCanvas::initGL();

	m_scene->Load(NULL);

	// for Camera reset when init
	Refresh();
}

void StageCanvas2D::onDraw()
{
	m_scene->Draw();

	m_stage->getEditOP()->OnDraw();
}

}
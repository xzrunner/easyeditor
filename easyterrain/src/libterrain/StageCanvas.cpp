#include "StageCanvas.h"

#include "DemoTriNetwork.h"
#include "DemoBruteForce.h"
#include "DemoBruteForceTex.h"

namespace libterrain
{

StageCanvas::StageCanvas(d2d::EditPanel* stage)
	: e3d::StageCanvas(stage)
{
	setBgColor(d2d::BLACK);

//	m_demo = new DemoTriNetwork(GetCamera3());
//	m_demo = new DemoBruteForce(GetCamera3());
	m_demo = new DemoBruteForceTex(GetCamera3());
}

void StageCanvas::initGL()
{
	e3d::StageCanvas::initGL();

	m_demo->Load();

	// for Camera reset when init
	Refresh();
}

void StageCanvas::onDraw()
{
//	e3d::DrawCross(vec3(0, 0, 0), vec3(100, 100, 100), d2d::LIGHT_RED);

	m_demo->Draw();

//	e3d::LogViewer::Instance()->Draw();
}

}
#include "StageCanvas.h"
#include "QuadScene.h"

#include <wx/dcbuffer.h>

namespace epseudo3d
{

BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
END_EVENT_TABLE()

StageCanvas::StageCanvas(d2d::EditPanel* stage)
	: e3d::StageCanvas(stage)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(100);

	setBgColor(d2d::BLACK);

	m_scene = new QuadScene(this);
}

void StageCanvas::StoreScene() const
{
}

void StageCanvas::LoadScene()
{

}

void StageCanvas::initGL()
{
	e3d::StageCanvas::initGL();

	m_scene->Load();
	InitCamera();

	// for Camera reset when init
	Refresh();
}

void StageCanvas::onDraw()
{
	m_scene->Draw();
}

void StageCanvas::OnDrawDC() const
{
	m_scene->DebugDraw();
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
	Refresh();
}

void StageCanvas::InitCamera()
{
	e3d::Camera& cam = GetCamera3();

//	cam.Translate(-cam.GetToward() * 20);
//	cam.Translate(cam.GetLeft() * 10);

	cam.Translate(-cam.GetToward() * 15);

	Refresh();
}

}
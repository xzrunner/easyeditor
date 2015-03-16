#include "StageCanvas.h"

//#include "Mapping3DScene.h"
#include "Projection2DScene.h"

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

//	m_scene = new Mapping3DScene(this);
	m_scene = new Projection2DScene(this);
}

void StageCanvas::StoreScene(const char* filename) const
{
	m_scene->Store(filename);
}

void StageCanvas::LoadScene(const char* filename)
{
	m_scene->Load(filename);
}

void StageCanvas::GetScreenSize(int* width, int* height) const
{
	*width = m_screen_width;
	*height = m_screen_height;
}

void StageCanvas::initGL()
{
	e3d::StageCanvas::initGL();

	m_scene->Load(NULL);
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
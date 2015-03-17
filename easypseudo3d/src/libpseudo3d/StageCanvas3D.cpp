#include "StageCanvas3D.h"

//#include "Mapping3DScene.h"
#include "Projection3DScene.h"

#include <wx/dcbuffer.h>

namespace epseudo3d
{

BEGIN_EVENT_TABLE(StageCanvas3D, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas3D::onTimer)
END_EVENT_TABLE()

StageCanvas3D::StageCanvas3D(d2d::EditPanel* stage)
	: e3d::StageCanvas(stage)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(100);

	setBgColor(d2d::BLACK);

//	m_scene = new Mapping3DScene(this);
	m_scene = new Projection3DScene(this);
}

void StageCanvas3D::StoreScene(const char* filename) const
{
	m_scene->Store(filename);
}

void StageCanvas3D::LoadScene(const char* filename)
{
	m_scene->Load(filename);
}

void StageCanvas3D::GetScreenSize(int* width, int* height) const
{
	*width = m_screen_width;
	*height = m_screen_height;
}

void StageCanvas3D::initGL()
{
	e3d::StageCanvas::initGL();

	m_scene->Load(NULL);

	// for Camera reset when init
	Refresh();
}

void StageCanvas3D::onDraw()
{
	m_scene->Draw();
}

void StageCanvas3D::OnDrawDC() const
{
	m_scene->DebugDraw();
}

void StageCanvas3D::onTimer(wxTimerEvent& event)
{
	Refresh();
}

}
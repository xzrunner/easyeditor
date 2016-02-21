#include "StageCanvas3D.h"

//#include "Mapping3DScene.h"
#include "Projection3DScene.h"

#include <ee/OrthoCanvas.h>

#include <wx/dcbuffer.h>

namespace epseudo3d
{

BEGIN_EVENT_TABLE(StageCanvas3D, ee::OrthoCanvas)
	EVT_TIMER(TIMER_ID, StageCanvas3D::OnTimer)
END_EVENT_TABLE()

StageCanvas3D::StageCanvas3D(wxWindow* stage_wnd, ee::EditPanelImpl* stage)
	: e3d::StageCanvas(stage_wnd, stage)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(100);

	SetBgColor(ee::BLACK);

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
	*width = m_width;
	*height = m_height;
}

void StageCanvas3D::OnDrawSprites() const
{
	m_scene->Draw();
}

//void StageCanvas3D::OnDrawDC() const
//{
//	m_scene->DebugDraw();
//}

void StageCanvas3D::OnTimer(wxTimerEvent& event)
{
	Refresh();
}

}
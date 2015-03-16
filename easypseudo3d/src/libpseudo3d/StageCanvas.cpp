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

	m_scene = new QuadScene();
}

void StageCanvas::StoreScene() const
{
	const e3d::Camera& cam = GetCamera3();
	mat4 mat_modelview = cam.GetModelViewMat();

// 	vec3 vertex1 = m_mat_projection * mat_modelview * vec3(5, 5, 5);
// 	vec3 vertex2 = m_mat_projection * mat_modelview * vec3(-5, 5, 5);
// 	vec3 vertex3 = m_mat_projection * mat_modelview * vec3(5, -5, 5);
// 	vec3 vertex4 = m_mat_projection * mat_modelview * vec3(5, 5, -5);
	vec3 vertex1 = mat_modelview * vec3(5, 5, 5);
	vec3 vertex2 = mat_modelview * vec3(-5, 5, 5);
	vec3 vertex3 = mat_modelview * vec3(5, -5, 5);
	vec3 vertex4 = mat_modelview * vec3(5, 5, -5);

	int w, h;
	cam.GetScreenSize(&w, &h);
	ivec2 v1 = e3d::ViewFrustum::TransPos3ProjectToScreen(vertex1, w, h);
	ivec2 v2 = e3d::ViewFrustum::TransPos3ProjectToScreen(vertex2, w, h);
	ivec2 v3 = e3d::ViewFrustum::TransPos3ProjectToScreen(vertex3, w, h);
	ivec2 v4 = e3d::ViewFrustum::TransPos3ProjectToScreen(vertex4, w, h);

	int zz = 0;
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

void StageCanvas::onSize(int w, int h)
{
	e3d::StageCanvas::onSize(w, h);

	float hh = 1.0f * h / w;
	m_mat_projection = mat4::Perspective(-1, 1, -hh, hh, 
		e3d::Camera::CAM_NEAR, e3d::Camera::CAM_FAR);
}

void StageCanvas::onDraw()
{
	m_scene->Draw();

	//////////////////////////////////////////////////////////////////////////

// 	const e3d::Camera& cam = GetCamera3();
// 	mat4 mat_modelview = cam.GetModelViewMat();
// 
// 	int w, h;
// 	cam.GetScreenSize(&w, &h);
// 
// 	wxBufferedPaintDC dc(this);
// 
// 	dc.SetPen(*wxWHITE_PEN);
// 	dc.SetBrush(*wxWHITE_BRUSH);
// 	dc.DrawRectangle(100, 100, 200, 400);

// 	// 	vec3 vertex1 = m_mat_projection * mat_modelview * vec3(5, 5, 5);
// 	// 	vec3 vertex2 = m_mat_projection * mat_modelview * vec3(-5, 5, 5);
// 	// 	vec3 vertex3 = m_mat_projection * mat_modelview * vec3(5, -5, 5);
// 	// 	vec3 vertex4 = m_mat_projection * mat_modelview * vec3(5, 5, -5);
// 	vec3 vertex1 = mat_modelview * vec3(5, 5, 5);
// 	vec3 vertex2 = mat_modelview * vec3(-5, 5, 5);
// 	vec3 vertex3 = mat_modelview * vec3(5, -5, 5);
// 	vec3 vertex4 = mat_modelview * vec3(5, 5, -5);
// 
// 	ivec2 v1 = e3d::ViewFrustum::TransPos3ProjectToScreen(vertex1, w, h);
// 	ivec2 v2 = e3d::ViewFrustum::TransPos3ProjectToScreen(vertex2, w, h);
// 	ivec2 v3 = e3d::ViewFrustum::TransPos3ProjectToScreen(vertex3, w, h);
// 	ivec2 v4 = e3d::ViewFrustum::TransPos3ProjectToScreen(vertex4, w, h);
// 
// 	int zz = 0;
}

void StageCanvas::OnDrawDC() const
{
// 	wxPaintDC dc(this);
// 
// 	wxMemoryDC dc;
// 	dc.SetPen(*wxWHITE_PEN);
// 	dc.SetBrush(*wxWHITE_BRUSH);
// 	dc.DrawRectangle(100, 100, 200, 400);
}

void StageCanvas::onTimer(wxTimerEvent& event)
{
// 	wxMemoryDC dc;
// 	dc.SetPen(*wxWHITE_PEN);
// 	dc.SetBrush(*wxWHITE_BRUSH);
// 	dc.DrawRectangle(100, 100, 200, 400);

	Refresh();
}

void StageCanvas::InitCamera()
{
	e3d::Camera& cam = GetCamera3();

//	cam.Translate(-cam.GetToward() * 20);
//	cam.Translate(cam.GetLeft() * 10);

	cam.Translate(-cam.GetToward() * 10);

	Refresh();
}

}
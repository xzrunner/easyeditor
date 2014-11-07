#include "StageCanvas.h"
#include "ShaderMgr.h"

namespace e3d
{

StageCanvas::StageCanvas(d2d::EditPanel* editPanel)
	: d2d::GLCanvas(editPanel)
{
}

void StageCanvas::Refresh()
{
	ShaderMgr::Instance()->SetModelView(m_camera3.GetModelViewMat());

	d2d::GLCanvas::Refresh();
}

void StageCanvas::initGL()
{
	d2d::GLCanvas::initGL();
	ShaderMgr::Instance()->SetModelView(m_camera3.GetModelViewMat());
}

void StageCanvas::onSize(int w, int h)
{
	glViewport(0, 0, w, h);
	m_screen.SetSize(w, h);
	m_screen.SetCamera();

	m_camera3.SetScreenSize(w, h);

	ShaderMgr::Instance()->SetProjection(w, h);
	ShaderMgr::Instance()->SetModelView(m_camera3.GetModelViewMat());
}

}
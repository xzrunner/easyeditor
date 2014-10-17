#include "StageCanvas.h"
#include "ShaderMgr.h"

namespace e3d
{

StageCanvas::StageCanvas(d2d::EditPanel* editPanel)
	: d2d::GLCanvas(editPanel)
{
}

void StageCanvas::onSize(int w, int h)
{
	glViewport(0, 0, w, h);
	m_screen.SetSize(w, h);
	m_screen.SetCamera();

	m_camera3.SetScreenSize(w, h);

	ShaderMgr::Instance()->SetProjection(w, h);
}

void StageCanvas::SetCameraRotation(const mat4& rot) 
{
	m_camera3.SetRotate(rot);
}

}
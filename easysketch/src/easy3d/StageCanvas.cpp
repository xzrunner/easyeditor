#include "StageCanvas.h"
#include "ShaderMgr.h"

namespace e3d
{

StageCanvas::StageCanvas(d2d::EditPanel* editPanel)
	: GLCanvas(editPanel)
{
}

void StageCanvas::onSize(int w, int h)
{
	glViewport(0, 0, w, h);
	m_screen.SetSize(w, h);
	m_screen.SetCamera();

	ShaderMgr::Instance()->SetProjection(w, h);
}

}
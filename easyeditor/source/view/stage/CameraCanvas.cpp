#include "CameraCanvas.h"
#include "Camera.h"
#include "GL.h"
#include "RenderContextStack.h"
#include "CameraMgr.h"

namespace ee
{

CameraCanvas::CameraCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
						 wxGLContext* glctx, bool use_context_stack)
	: TwoPassCanvas(stage_wnd, stage, glctx, use_context_stack)
{
}

void CameraCanvas::OnSize(int w, int h)
{
//  	glViewport(0, 0, w, h);
//  
//  	glMatrixMode(GL_PROJECTION);
//  	glLoadIdentity();
//  
//  	const float hWidth = w * m_camera->getScale() * 0.5f,
//  		hHeight = h * m_camera->getScale() * 0.5f;
//  	glOrtho(
//  		m_camera->getCenter().x - hWidth, 
//  		m_camera->getCenter().x + hWidth, 
//  		m_camera->getCenter().y - hHeight,
//  		m_camera->getCenter().y + hHeight,
//  		0,
//  		1
//  		);
//  
//  	glMatrixMode(GL_MODELVIEW);
//  	glLoadIdentity();

	//////////////////////////////////////////////////////////////////////////

// 	glViewport(0, 0, w, h);
// 	sm::vec2 scale;
// 	scale.x = scale.y = 1.0f / m_camera->getScale();
// 	m_screen.SetSize(w, h);
// 	m_screen.SetCamera(-m_camera->getCenter(), scale);

	//////////////////////////////////////////////////////////////////////////

	TwoPassCanvas::OnSize(w, h);

 	m_screen.SetSize(w, h);
 	m_screen.SetCamera();

	CameraMgr::Instance()->GetCamera()->UpdateModelView();

	RenderContextStack::Instance()->SetProjection(w, h);
	GL::Viewport(0, 0, w, h);
}

}
#include "OrthoCanvas.h"
#include "Camera.h"

#include "render/ShaderMgr.h"
#include "render/GL.h"
#include "render/RenderContext.h"

namespace d2d
{

OrthoCanvas::OrthoCanvas(wxWindow* stage_wnd, EditPanelImpl* stage)
	: TwoPassCanvas(stage_wnd, stage)
{
}

void OrthoCanvas::OnSize(int w, int h)
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
// 	Vector scale;
// 	scale.x = scale.y = 1.0f / m_camera->getScale();
// 	m_screen.SetSize(w, h);
// 	m_screen.SetCamera(-m_camera->getCenter(), scale);

	//////////////////////////////////////////////////////////////////////////

	d2d::TwoPassCanvas::OnSize(w, h);

 	m_screen.SetSize(w, h);
 	m_screen.SetCamera();

	m_camera->UpdateModelView();

	RenderContext::GetCurrContext()->SetProjection(w, h);
	GL::Viewport(0, 0, w, h);
}

} // d2d
#include "OrthoCanvas.h"
#include "Camera.h"

#include "view/EditPanel.h"
#include "render/ShaderNew.h"

#include <gl/glu.h>

namespace d2d
{

OrthoCanvas::OrthoCanvas(EditPanel* editPanel)
	: GLCanvas(editPanel)
{
}

void OrthoCanvas::onSize(int w, int h)
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

 	glViewport(0, 0, w, h);
 	m_screen.SetSize(w, h);
 	m_screen.SetCamera();

	m_camera->UpdateModelView();
	ShaderNew::Instance()->SetProjection(w, h);
}
} // d2d
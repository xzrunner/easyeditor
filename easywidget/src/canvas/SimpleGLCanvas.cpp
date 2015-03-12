#include "SimpleGLCanvas.h"

#include <easy2d.h>
#include <gl/GLee.h>

namespace ewidget
{

SimpleGLCanvas::SimpleGLCanvas(egui::Window* parent)
	: GLCanvas(parent)
{
}

void SimpleGLCanvas::OnSize(int w, int h)
{
 	glViewport(0, 0, w, h);
 
 	e2d::ShaderMgr* shader = e2d::ShaderMgr::Instance();
  	shader->SetModelView(vec2(0, 0), 1);
  	shader->SetProjection(w, h);
}

}
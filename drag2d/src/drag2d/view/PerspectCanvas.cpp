#include "PerspectCanvas.h"

#include <gl/glu.h>

namespace d2d
{

GLfloat cameraPos[] = {0, -400.0f, 400.0f, 1.0f};

PerspectCanvas::PerspectCanvas(EditPanel* editPanel)
	: GLCanvas(editPanel)
{
}

void PerspectCanvas::onSize(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

 	GLfloat fAspect = (GLfloat)w / (GLfloat)h;
 	gluPerspective(60.0f, fAspect, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}
} // d2d
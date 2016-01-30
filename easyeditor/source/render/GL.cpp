#include "GL.h"

#include <gl/glew.h>

namespace ee
{

void GL::Viewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
}

}
#include "PrimitiveDrawNew.h"
#include "ShaderNew.h"

#include <GL/GLee.h>

namespace d2d
{

void PrimitiveDrawNew::Polyline(const float *coords, size_t count)
{
	//glLineWidth(4);

	ShaderNew::Instance()->Commit();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, (const GLvoid*)coords);
	glDrawArrays(GL_LINE_STRIP, 0, count);
	glDisableClientState(GL_VERTEX_ARRAY);
}

}
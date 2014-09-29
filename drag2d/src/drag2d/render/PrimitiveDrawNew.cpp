#include "PrimitiveDrawNew.h"
#include "ShaderNew.h"

#include <GL/GLee.h>

namespace d2d
{

void PrimitiveDrawNew::Draw(int type, const float *coords, size_t count)
{
	ShaderNew* shader = ShaderNew::Instance();
	shader->shape();
	shader->Commit();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, (const GLvoid*)coords);
	glDrawArrays(type, 0, count);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void PrimitiveDrawNew::SetLineWidth(float width)
{
	ShaderNew::Instance()->shape();
	glLineWidth(width);
}

void PrimitiveDrawNew::SetColor(const Colorf& col)
{
	ShaderNew::Instance()->shape();
	ShaderNew::Instance()->SetShapeColor(col);
}

void PrimitiveDrawNew::SetPointSize(float size)
{
	ShaderNew::Instance()->shape();
	glPointSize(size);
}

void PrimitiveDrawNew::LineStypeBegin(const LineStyle& style)
{
	if (style == LS_DEFAULT) {
		return;
	}

	ShaderNew::Instance()->shape();

	glEnable(GL_LINE_STIPPLE);
	switch (style)
	{
	case LS_DOT:
		glLineStipple(1, 0x0101);
		break;
	case LS_DASH:
		glLineStipple(1, 0x00FF);
		break;
	case LS_DOT_DASH:
		glLineStipple(1, 0x1c47);
		break;
	}
}

void PrimitiveDrawNew::LineStypeEnd(const LineStyle& style)
{
	if (style == LS_DEFAULT) {
		return;
	}

	ShaderNew::Instance()->shape();
	glDisable(GL_LINE_STIPPLE);
}

}
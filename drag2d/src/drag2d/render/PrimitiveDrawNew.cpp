#include "PrimitiveDrawNew.h"
#include "ShaderMgr.h"
#include "ShapeShader.h"

#include <gl/glew.h>

namespace d2d
{

static const int MAX_COLORS = 4096 * 10;
static float COLORS[MAX_COLORS * 4];

void PrimitiveDrawNew::Draw(int type, const float *coords, size_t count)
{
	ShaderMgr* shader = ShaderMgr::Instance();
	shader->shape();
	shader->Commit();

	// prepare colors
	const Colorf& col = ShaderMgr::Instance()->GetShapeShader()->GetColor();
	for (int i = 0; i < count * 4; i += 4) {
		memcpy(&COLORS[i], &col.r, sizeof(float) * 4);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, (const GLvoid*)coords);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, (const GLvoid*)COLORS);

	glDrawArrays(type, 0, count);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void PrimitiveDrawNew::SetLineWidth(float width)
{
	ShaderMgr::Instance()->shape();
	glLineWidth(width);
}

void PrimitiveDrawNew::SetColor(const Colorf& col)
{
	ShaderMgr::Instance()->shape();
	ShaderMgr::Instance()->SetShapeColor(col);
}

void PrimitiveDrawNew::SetPointSize(float size)
{
	ShaderMgr::Instance()->shape();
	glPointSize(size);
}

void PrimitiveDrawNew::LineStypeBegin(const LineStyle& style)
{
	if (style == LS_DEFAULT) {
		return;
	}

	ShaderMgr::Instance()->shape();

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

	ShaderMgr::Instance()->shape();
	glDisable(GL_LINE_STIPPLE);
}

}
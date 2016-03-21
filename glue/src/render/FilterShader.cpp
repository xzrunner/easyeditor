#include "FilterShader.h"

#include <sl_filter.h>

namespace glue
{

FilterShader::FilterShader()
{
}

FilterShader::~FilterShader()
{
}

void FilterShader::Load()
{
	sl_filter_load();
	sl_filter_modelview(0, 0, 1, 1);
}

void FilterShader::Unload()
{
	sl_filter_unload();
}

void FilterShader::Bind()
{
	sl_filter_bind();
}

void FilterShader::Unbind()
{
	sl_filter_unbind();
}

void FilterShader::OnSize(int width, int height)
{
	sl_filter_projection(width, height);
}

void FilterShader::Commit()
{
	sl_filter_commit();
}

void FilterShader::Reset()
{
}

void FilterShader::SetProjection(int width, int height)
{
	sl_filter_projection(width, height);
}

void FilterShader::SetModelView(const vec2& offset, float scale)
{
	sl_filter_modelview(offset.x, offset.y, scale, scale);
}

void FilterShader::Draw(const vec2 vertices[4], const vec2 texcoords[4], int texid)
{
	sl_filter_draw(&vertices[0].x, &texcoords[0].x, texid);
}

}
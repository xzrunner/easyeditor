#include "ShapeShader.h"

#include <sl_shape.h>

namespace glue
{

ShapeShader::ShapeShader()
{
}

ShapeShader::~ShapeShader()
{
}

void ShapeShader::Load()
{
	sl_shape_load();
	sl_shape_modelview(0, 0, 1, 1);
}

void ShapeShader::Unload()
{
	sl_shape_unload();
}

void ShapeShader::Bind()
{
	sl_shape_bind();
}

void ShapeShader::Unbind()
{
	sl_shape_unbind();
}

void ShapeShader::OnSize(int width, int height)
{
	sl_shape_projection(width, height);
}

void ShapeShader::Commit()
{
//	sl_shape_commit();
}

void ShapeShader::Reset()
{
}

void ShapeShader::SetProjection(int width, int height)
{
	sl_shape_projection(width, height);
}

void ShapeShader::SetModelView(const vec2& offset, float scale)
{
	sl_shape_modelview(offset.x, offset.y, scale, scale);
}

}
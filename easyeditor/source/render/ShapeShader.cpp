#include "ShapeShader.h"
#include "trans_color.h"

#include <shaderlab.h>

#include <gl/glew.h>

#include <string>

namespace ee
{

ShapeShader::ShapeShader()
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

void ShapeShader::SetModelView(const Vector& offset, float scale)
{
	sl_shape_modelview(offset.x, offset.y, scale, scale);
}

void ShapeShader::SetProjection(int width, int height)
{
	sl_shape_projection(width, height);
}

void ShapeShader::Commit()
{
	sl_shape_commit();
}

void ShapeShader::SetColor(const Colorf& col)
{
	sl_shape_color(color2int(col, PT_ABGR));
}

}
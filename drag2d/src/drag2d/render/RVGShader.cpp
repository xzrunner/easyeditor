#include "RVGShader.h"

#include "common/color_trans.h"

#include <rvg_shader.h>

namespace d2d
{

RVGShader::RVGShader()
{
}

void RVGShader::Load()
{
	rvg_shader_init();
	rvg_shader_load();
}

void RVGShader::Unload()
{
	rvg_shader_unload();
}

void RVGShader::Bind()
{
	rvg_shader_bind();
}

void RVGShader::Unbind()
{
	rvg_shader_unbind();
}

void RVGShader::SetProjection(int width, int height)
{
	rvg_shader_projection(width, height);
}

void RVGShader::Commit()
{
	rvg_shader_flush();
}

void RVGShader::SetModelView(const Vector& offset, float scale)
{
	rvg_shader_modelview(offset.x, offset.y, scale);
}

void RVGShader::SetColor(const Colorf& col)
{
	int icol = trans_color2int(col, PT_RGBA);
	rvg_shader_color(icol);
}

void RVGShader::LoadShader()
{
	rvg_shader_init();
	rvg_shader_load();
}

}
#include "BlendShader.h"

#include <sl_blend.h>

namespace glue
{

BlendShader::BlendShader()
{
}

BlendShader::~BlendShader()
{
}

void BlendShader::Load()
{
	sl_blend_load();
	sl_blend_modelview(0, 0, 1, 1);
}

void BlendShader::Unload()
{
	sl_blend_unload();
}

void BlendShader::Bind()
{
	sl_blend_bind();
}

void BlendShader::Unbind()
{
	sl_blend_unbind();
}

void BlendShader::OnSize(int width, int height)
{
	sl_blend_projection(width, height);
}

void BlendShader::Commit()
{
	sl_blend_commit();
}

void BlendShader::Reset()
{
}

void BlendShader::SetProjection(int width, int height)
{
	sl_blend_projection(width, height);
}

void BlendShader::SetModelView(const vec2& offset, float scale)
{
	sl_blend_modelview(offset.x, offset.y, scale, scale);
}

void BlendShader::SetColor(uint32_t color, uint32_t additive)
{
	sl_blend_set_color(color, additive);
}

void BlendShader::SetMode(BlendMode mode)
{
	sl_blend_set_mode((SL_BLEND_MODE)mode);
}

void BlendShader::Draw(const vec2 vertices[4], const vec2 texcoords[4], 
					   const vec2 texcoords_base[4], int tex_blend, int tex_base)
{
	sl_blend_draw(&vertices[0].x, &texcoords[0].x, &texcoords_base[0].x, tex_blend, tex_base);
}

}
#include "BlendShader.h"
#include "BlendModes.h"
#include "ColorTrans.h"
#include "trans_color.h"

#include <sl_blend.h>

namespace ee
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

void BlendShader::SetProjection(int width, int height)
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

void BlendShader::SetModelView(const Vector& offset, float scale)
{
	sl_blend_modelview(offset.x, offset.y, scale, scale);
}

void BlendShader::SetMode(const std::string& str)
{
	BlendMode mode = BlendModes::Instance()->GetModeFromNameEN(str);
	sl_blend_set_mode(SL_BLEND_MODE(mode));
}

void BlendShader::SetColor(const ColorTrans& color)
{
	sl_blend_set_color(color2int(color.multi, PT_ABGR), 
		color2int(color.add, PT_ABGR));
}

void BlendShader::Draw(const Vector vertices[4], const Vector texcoords[4], 
							const Vector texcoords_base[4], int texid, int texid_base)
{
	sl_blend_draw(&vertices[0].x, &texcoords[0].x, &texcoords_base[0].x, texid, texid_base);
}

}
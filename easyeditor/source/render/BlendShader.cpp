#include "BlendShader.h"

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

// 	void SetBaseTexID(int tex);
// 
// 	void SetBlendMode(const std::string& mode);

void BlendShader::DrawBlend(const Vector vertices[4], const Vector texcoords[4], 
							const Vector texcoords_base[4], int texid, int texid_base)
{
	sl_blend_draw(&vertices[0].x, &texcoords[0].x, &texcoords_base[0].x, texid, texid_base);
}

}
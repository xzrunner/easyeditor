#include "SpriteShader.h"
#include "RenderColor.h"
#include "trans_color.h"

#include <sl_sprite.h>

namespace ee
{

SpriteShader::SpriteShader()
{
}

SpriteShader::~SpriteShader()
{
}

void SpriteShader::Load()
{
 	sl_sprite_load();
// 	sl_sprite_modelview(0, 0, 1, 1);
}

void SpriteShader::Unload()
{
	sl_sprite_unload();
}

void SpriteShader::Bind()
{
	sl_sprite_bind();
}

void SpriteShader::Unbind()
{
	sl_sprite_unbind();
}

void SpriteShader::SetModelView(const Vector& offset, float scale)
{
	sl_sprite_modelview(offset.x, offset.y, scale, scale);
}

void SpriteShader::SetProjection(int width, int height)
{
	sl_sprite_projection(width, height);
}

void SpriteShader::Commit()
{
	sl_sprite_commit();
}

void SpriteShader::Reset()
{
}

//void SpriteShader::SetBufferData(bool open) 
//{
//	m_open_buffer_data = open;
//}
//
//bool SpriteShader::IsOpenBufferData() const 
//{ 
//	return m_open_buffer_data; 
//}

void SpriteShader::SetColor(const RenderColor& color)
{
	sl_sprite_set_color(color2int(color.multi, PT_ABGR), 
		color2int(color.add, PT_ABGR));
	sl_sprite_set_map_color(color2int(color.r, PT_ABGR), 
		color2int(color.g, PT_ABGR), 
		color2int(color.b, PT_ABGR));
}

//void SpriteShader::Draw(const float vb[16], int texid)
//{
//	SetTexID(texid);
//
//	CopyVertex(vb);
//	if (++m_count >= m_max_commbine) {
//		Commit();
//	}
//}

void SpriteShader::Draw(const Vector vertices[4], const Vector texcoords[4], int texid)
{
	sl_sprite_draw(&vertices[0].x, &texcoords[0].x, texid);
}

}
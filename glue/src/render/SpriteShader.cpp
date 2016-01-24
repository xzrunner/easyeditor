#include "SpriteShader.h"

#include <sl_sprite.h>

namespace glue
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
	sl_sprite_modelview(0, 0, 1, 1);
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

void SpriteShader::OnSize(int width, int height)
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

void SpriteShader::SetProjection(int width, int height)
{
	sl_sprite_projection(width, height);
}

void SpriteShader::SetModelView(const vec2& offset, float scale)
{
	sl_sprite_modelview(offset.x, offset.y, scale, scale);
}

void SpriteShader::SetColor(const Colorf& color, const Colorf& additive)
{
//	sl_sprite_set_color();
}

void SpriteShader::SetMapColor(const Colorf& rmap, const Colorf& gmap, const Colorf& bmap)
{
	
}

void SpriteShader::Draw(const vec2 vertices[4], const vec2 texcoords[4], int texid)
{
	sl_sprite_draw(&vertices[0].x, &texcoords[0].x, texid);
}

}
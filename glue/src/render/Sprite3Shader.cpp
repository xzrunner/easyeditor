#include "Sprite3Shader.h"

#include <sl_sprite3.h>

#include <assert.h>

namespace glue
{

Sprite3Shader::Sprite3Shader()
{
}

Sprite3Shader::~Sprite3Shader()
{
}

void Sprite3Shader::Load()
{
	sl_sprite3_load();
	sl_sprite3_modelview(0, 0, 1, 1);
}

void Sprite3Shader::Unload()
{
	sl_sprite3_unload();
}

void Sprite3Shader::Bind()
{
	sl_sprite3_bind();
}

void Sprite3Shader::Unbind()
{
	sl_sprite3_unbind();
}

void Sprite3Shader::OnSize(int width, int height)
{
	sl_sprite3_projection(width, height, NEAR, FAR);
}

void Sprite3Shader::Commit()
{
	sl_sprite3_commit();
}

void Sprite3Shader::Reset()
{
}

void Sprite3Shader::SetProjection(int width, int height)
{
	sl_sprite3_projection(width, height, NEAR, FAR);
}

void Sprite3Shader::SetModelView(const vec2& offset, float scale)
{
	sl_sprite3_modelview(offset.x, offset.y, scale, scale);
}

void Sprite3Shader::Draw(const std::vector<vec3>& vertices, const std::vector<vec2>& texcoords, int texid)
{
	assert(vertices.size() == texcoords.size());
	sl_sprite3_draw(&vertices[0].x, &texcoords[0].x, texid, vertices.size());
}

}
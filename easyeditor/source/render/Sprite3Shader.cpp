#include "Sprite3Shader.h"
#include "RenderColor.h"
#include "trans_color.h"
#include "Pseudo3DCamera.h"

#include <sl_sprite3.h>

namespace ee
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

void Sprite3Shader::SetModelView(const vec3& pos, float angle)
{
	sl_sprite3_modelview(pos.x, pos.y, pos.z, angle);
}

void Sprite3Shader::SetProjection(int width, int height)
{
	sl_sprite3_projection(width, height, Pseudo3DCamera::NEAR_CLIP, Pseudo3DCamera::FAR_CLIP);
}

void Sprite3Shader::Commit()
{
	sl_sprite3_commit();
}

void Sprite3Shader::Reset()
{
}

void Sprite3Shader::SetColor(const RenderColor& color)
{
	sl_sprite3_set_color(color2int(color.multi, PT_ABGR), 
		color2int(color.add, PT_ABGR));
}

void Sprite3Shader::Draw(const std::vector<vec3>& positions, const std::vector<Vector>& texcoords, int texid)
{
	sl_sprite3_draw(&positions[0].x, &texcoords[0].x, texid, positions.size());
}

}
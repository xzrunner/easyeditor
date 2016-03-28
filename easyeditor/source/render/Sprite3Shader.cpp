#include "Sprite3Shader.h"
#include "ColorTrans.h"
#include "trans_color.h"

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
	sl_sprite3_projection(width, height, NEAR, FAR);
}

void Sprite3Shader::Commit()
{
	sl_sprite3_commit();
}

void Sprite3Shader::Reset()
{
}

void Sprite3Shader::SetColor(const ColorTrans& color)
{
// 	sl_sprite3_set_color(color2int(color.multi, PT_ABGR), 
// 		color2int(color.add, PT_ABGR));
// 	sl_sprite3_set_map_color(color2int(color.r, PT_ABGR), 
// 		color2int(color.g, PT_ABGR), 
// 		color2int(color.b, PT_ABGR));
}

void Sprite3Shader::Draw(const std::vector<vec3>& positions, const std::vector<Vector>& texcoords, int texid)
{
	sl_sprite3_draw(&positions[0].x, &texcoords[0].x, texid, positions.size());
}

}
#include "LightingShader.h"

#include <sl_lighting.h>
#include <sl_math.h>
#include <m3_mesh.h>

#include <assert.h>

namespace glue
{

LightingShader::LightingShader()
{
}

LightingShader::~LightingShader()
{
}

void LightingShader::Load()
{
	sl_lighting_load();
	sl_lighting_modelview(0, 0, 1, 1);
}

void LightingShader::Unload()
{
	sl_lighting_unload();
}

void LightingShader::Bind()
{
	sl_lighting_bind();
}

void LightingShader::Unbind()
{
	sl_lighting_unbind();
}

void LightingShader::OnSize(int width, int height)
{
	sl_lighting_projection(width, height, VIEW_NEAR, VIEW_FAR);
}

void LightingShader::Commit()
{
	sl_lighting_commit();
}

void LightingShader::Reset()
{
}

void LightingShader::SetProjection(int width, int height)
{
	sl_lighting_projection(width, height, VIEW_NEAR, VIEW_FAR);
}

void LightingShader::SetModelView(const vec2& offset, float scale)
{
	sl_lighting_modelview(offset.x, offset.y, scale, scale);
}

void LightingShader::SetMaterial(const vec3& ambient, const vec3& diffuse, 
								 const vec3& specular, float shininess)
{
	sl_vec3 sl_ambient, sl_diffuse, sl_specular;
	memcpy(&sl_ambient.x, ambient.xyz, sizeof(sl_ambient));
	memcpy(&sl_diffuse.x, diffuse.xyz, sizeof(sl_diffuse));
	memcpy(&sl_specular.x, specular.xyz, sizeof(sl_specular));
	sl_lighting_set_material(&sl_ambient, &sl_diffuse, &sl_specular, shininess);
}

void LightingShader::SetNormalMatrix(const mat3& mat)
{
	sl_mat3 sl_mat;
	memcpy(sl_mat.x, mat.x, sizeof(sl_mat));
	sl_lighting_set_normal_matrix(&sl_mat);
}

void LightingShader::SetLightPosition(const vec3& pos)
{
	sl_vec3 sl_pos;
	memcpy(&sl_pos.x, pos.xyz, sizeof(sl_pos));
	sl_lighting_set_light_position(&sl_pos);
}

void LightingShader::Draw(m3_mesh* mesh)
{
	sl_lighting_draw(mesh->vertices, mesh->indices);
}

}
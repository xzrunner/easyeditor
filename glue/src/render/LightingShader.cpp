#include "LightingShader.h"

#include <sl_lighting.h>
#include <m3_mesh.h>

#include <assert.h>

namespace glue
{

static const float VIEW_FAR = 5000;
static const float VIEW_NEAR = 0.1f;

LightingShader::LightingShader()
{
}

LightingShader::~LightingShader()
{
}

void LightingShader::Load()
{
	sl_lighting_load();
	sl_lighting_modelview(&m_mat);
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

void LightingShader::SetModelView(const sm_mat4& mat)
{
	m_mat = mat;
	sl_lighting_modelview(&m_mat);
}

void LightingShader::SetMaterial(const vec3& ambient, const vec3& diffuse, 
								 const vec3& specular, float shininess)
{
	sm_vec3 sm_ambient, sm_diffuse, sm_specular;
	memcpy(&sm_ambient.x, ambient.xyz, sizeof(sm_ambient));
	memcpy(&sm_diffuse.x, diffuse.xyz, sizeof(sm_diffuse));
	memcpy(&sm_specular.x, specular.xyz, sizeof(sm_specular));
	sl_lighting_set_material(&sm_ambient, &sm_diffuse, &sm_specular, shininess);
}

void LightingShader::SetNormalMatrix(const sm_mat3& mat)
{
	sl_lighting_set_normal_matrix(&mat);
}

void LightingShader::SetLightPosition(const vec3& pos)
{
	sm_vec3 sm_pos;
	memcpy(&sm_pos.x, pos.xyz, sizeof(sm_pos));
	sl_lighting_set_light_position(&sm_pos);
}

void LightingShader::Draw(m3_mesh* mesh)
{
	sl_lighting_draw(mesh->vertices, mesh->indices);
}

}
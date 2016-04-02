#include "LightingShader.h"

#include <sl_lighting.h>
#include <m3_model.h>
#include <ds_array.h>

#include <assert.h>

namespace glue
{

LightingShader::LightingShader()
{
	sm_mat4_identity(&m_mat);
}

LightingShader::~LightingShader()
{
}

void LightingShader::Load()
{
	sl_lighting_load();
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
	sl_lighting_projection((float)width / height);
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
	sl_lighting_projection((float)width / height);
}

void LightingShader::SetModelView(const sm_mat4& mat)
{
	m_mat = mat;
	sl_lighting_modelview(&m_mat);
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

void LightingShader::Draw(m3_model* model)
{
	for (int i = 0, n = ds_array_size(model->meshes); i < n; ++i) {
		m3_mesh* mesh = *(m3_mesh**)ds_array_fetch(model->meshes, i);
		sl_lighting_set_material(&mesh->material.ambient, &mesh->material.diffuse, &mesh->material.specular, mesh->material.shininess);
		sl_lighting_draw(mesh->vertices, mesh->indices);
	}
}

}
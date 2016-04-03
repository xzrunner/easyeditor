#include "LightingShader.h"

#include <sl_model3.h>
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
	sl_model3_load();
}

void LightingShader::Unload()
{
	sl_model3_unload();
}

void LightingShader::Bind()
{
	sl_model3_bind();
}

void LightingShader::Unbind()
{
	sl_model3_unbind();
}

void LightingShader::OnSize(int width, int height)
{
	sl_model3_projection((float)width / height);
}

void LightingShader::Commit()
{
	sl_model3_commit();
}

void LightingShader::Reset()
{
}

void LightingShader::SetProjection(int width, int height)
{
	sl_model3_projection((float)width / height);
}

void LightingShader::SetModelView(const sm_mat4& mat)
{
	m_mat = mat;
	sl_model3_modelview(&m_mat);
}

void LightingShader::SetNormalMatrix(const sm_mat3& mat)
{
	sl_model3_set_normal_matrix(&mat);
}

void LightingShader::SetLightPosition(const vec3& pos)
{
	sm_vec3 sm_pos;
	memcpy(&sm_pos.x, pos.xyz, sizeof(sm_pos));
	sl_model3_set_light_position(&sm_pos);
}

void LightingShader::Draw(m3_model* model)
{
	for (int i = 0, n = ds_array_size(model->meshes); i < n; ++i) {
		m3_mesh* mesh = *(m3_mesh**)ds_array_fetch(model->meshes, i);
		sl_model3_set_material(&mesh->material.ambient, &mesh->material.diffuse, &mesh->material.specular, mesh->material.shininess);
		sl_model3_draw(mesh->vertices, mesh->indices);
	}
}

}
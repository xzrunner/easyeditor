#include "Sprite3Shader.h"

#include <sl_sprite3.h>
#include <c25_camera.h>

#include <assert.h>

namespace glue
{

static const float Z = -1000;
static const float ANGLE = -20;

Sprite3Shader::Sprite3Shader()
	: m_cam(NULL)
{
}

Sprite3Shader::~Sprite3Shader()
{
	if (m_cam) {
		c25_cam_release(m_cam);
	}
}

void Sprite3Shader::Load()
{
	sl_sprite3_load();
	if (m_cam) {
		sl_sprite3_modelview(c25_cam_get_modelview_mat(m_cam));
	}
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
	if (m_cam) {
		c25_cam_release(m_cam);
	}

	sm_vec3 pos;
	pos.x = pos.y = 0;
	pos.z = Z;
	m_cam = c25_cam_create(&pos, ANGLE, (float)width / height);

	sl_sprite3_projection(c25_cam_get_project_mat(m_cam));
	sl_sprite3_modelview(c25_cam_get_modelview_mat(m_cam));
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
	if (m_cam) {
		c25_cam_set_project(m_cam, (float)width / height);
		sl_sprite3_projection(c25_cam_get_project_mat(m_cam));
	}
}

void Sprite3Shader::SetModelView(const vec2& offset, float scale)
{
	if (m_cam) {
		sm_vec3 pos;
		pos.x = offset.x;
		pos.y = offset.y;
		pos.z = Z;
		c25_cam_set_modelview(m_cam, &pos, ANGLE);
		sl_sprite3_modelview(c25_cam_get_modelview_mat(m_cam));
	}
}

void Sprite3Shader::Draw(const std::vector<vec3>& vertices, const std::vector<vec2>& texcoords, int texid)
{
	assert(vertices.size() == texcoords.size());
	sl_sprite3_draw(&vertices[0].x, &texcoords[0].x, texid, vertices.size());
}

}
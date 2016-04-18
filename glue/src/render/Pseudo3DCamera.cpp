#include "Pseudo3DCamera.h"

#include <c25_camera.h>

namespace glue
{

//static const float Z = -1000;
static const float Z = -250;
static const float ANGLE = -20;

Pseudo3DCamera::Pseudo3DCamera()
	: m_cam(NULL)
{
}

Pseudo3DCamera::~Pseudo3DCamera()
{
	if (m_cam) {
		c25_cam_release(m_cam);
	}
}

void Pseudo3DCamera::OnSize(int width, int height)
{
	if (m_cam) {
		c25_cam_release(m_cam);
	}

	sm_vec3 pos;
	pos.x = pos.y = 0;
	pos.z = Z;
	m_cam = c25_cam_create(&pos, ANGLE, (float)width / height);
}

void Pseudo3DCamera::Reset()
{
	if (!m_cam) {
		return;
	}

	sm_vec3 pos;
	pos.x = pos.y = 0;
	pos.z = Z;
	c25_cam_set_pos(m_cam, &pos);

	c25_cam_set_angle(m_cam, ANGLE);
}

vec2 Pseudo3DCamera::TransPosScreenToProject(int x, int y, int width, int height) const
{
	sm_ivec2 screen;
	screen.x = x;
	screen.y = y;

	sm_vec2 world;
	c25_screen_to_world(m_cam, &world, &screen, width, height);

	return vec2(world.x, world.y);
}

vec2 Pseudo3DCamera::TransPosProjectToScreen(const vec3& proj, int width, int height) const
{
	sm_vec3 world;
	world.x = proj.x;
	world.y = proj.y;
	world.z = proj.z;

	sm_ivec2 screen;
	c25_world_to_screen(m_cam, &screen, &world, width, height);

	return vec2((float)screen.x, (float)screen.y);
}

const sm_mat4* Pseudo3DCamera::GetModelViewMat() const
{
	return m_cam ? c25_cam_get_modelview_mat(m_cam) : NULL;
}

const sm_mat4* Pseudo3DCamera::GetProjectMat() const
{
	return m_cam ? c25_cam_get_project_mat(m_cam) : NULL;
}

}
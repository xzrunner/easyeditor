#include "Pseudo3DCamera.h"
#include "panel_msg.h"

#include <c25_camera.h>
#include <sm_c_vector.h>
#include <gum/RenderContext.h>
#include <shaderlab/SubjectMVP3.h>

#include <wx/log.h>

namespace ee
{

static const float Z = -1000;
static const float ANGLE = -20;

// static const float Z = -193.5f;
// static const float ANGLE = -45;

Pseudo3DCamera::Pseudo3DCamera()
{
	sm_vec3 pos;
	pos.x = pos.y = 0;
	pos.z = Z;

	int w = gum::RenderContext::Instance()->GetWidth(),
		h = gum::RenderContext::Instance()->GetHeight();
	m_cam = c25_cam_create(&pos, ANGLE, (float)w / h);

	UpdateModelView();
}

Pseudo3DCamera::~Pseudo3DCamera()
{
	c25_cam_release(m_cam);
}

void Pseudo3DCamera::Reset()
{
	sm_vec3 pos;
	pos.x = pos.y = 0;
	pos.z = Z;
	c25_cam_set_pos(m_cam, &pos);

	c25_cam_set_angle(m_cam, ANGLE);

	UpdateModelView();
}

sm::vec2 Pseudo3DCamera::TransPosScreenToProject(int x, int y, int width, int height) const
{
	sm_ivec2 screen;
	screen.x = x;
	screen.y = y;

	sm_vec2 world;
	c25_screen_to_world(m_cam, &world, &screen, width, height);

	return sm::vec2(world.x, world.y);
}

sm::vec2 Pseudo3DCamera::TransPosProjectToScreen(const sm::vec2& proj, int width, int height) const
{
	return TransPosProjectToScreen(sm::vec3(proj.x, proj.y, 0), width, height);
}

sm::vec2 Pseudo3DCamera::TransPosProjectToScreen(const sm::vec3& proj, int width, int height) const
{
	sm_vec3 world;
	world.x = proj.x;
	world.y = proj.y;
	world.z = proj.z;

	sm_ivec2 screen;
	c25_world_to_screen(m_cam, &screen, &world, width, height);

	return sm::vec2(screen.x, screen.y);
}

void Pseudo3DCamera::UpdateModelView() const
{
	float angle = c25_cam_get_angle(m_cam);

	const sm_mat4* mat = GetModelViewMat();
	sl::SubjectMVP3::Instance()->NotifyModelview(*(const sm::mat4*)mat);
}

float Pseudo3DCamera::GetScale() const
{
	return 1;
}

const sm::vec2& Pseudo3DCamera::GetPosition() const
{
	return sm::vec2(0, 0);
}

void Pseudo3DCamera::Translate(const sm::vec3& offset)
{
	sm_vec3 vec;
	vec.x = offset.x;
	vec.y = offset.y;
	vec.z = offset.z;
	c25_cam_translate(m_cam, &vec);
	UpdateModelView();
}

void Pseudo3DCamera::Rotate(float da)
{
	c25_cam_rotate(m_cam, da);
	UpdateModelView();
	SetCanvasDirtySJ::Instance()->SetDirty();
}

float Pseudo3DCamera::GetAngle() const 
{
	return c25_cam_get_angle(m_cam);
}

const sm_vec3* Pseudo3DCamera::GetPos() const
{
	return c25_cam_get_pos(m_cam);
}

const sm_mat4* Pseudo3DCamera::GetModelViewMat() const
{
	return c25_cam_get_modelview_mat(m_cam);
}

const sm_mat4* Pseudo3DCamera::GetProjectMat() const
{
	return c25_cam_get_project_mat(m_cam);
}

}
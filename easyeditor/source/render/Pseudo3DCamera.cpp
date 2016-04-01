#include "Pseudo3DCamera.h"
#include "ShaderMgr.h"
#include "panel_msg.h"
#include "ScreenCache.h"

#include <sm.h>
#include <c25_camera.h>

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

	int w, h;
	ScreenCache::Instance()->GetSize(w, h);
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

	c25_cam_reset(m_cam, &pos, ANGLE);

	UpdateModelView();
}

Vector Pseudo3DCamera::TransPosScreenToProject(int x, int y, int width, int height) const
{
	sm_ivec2 screen;
	screen.x = x;
	screen.y = y;

	sm_vec2 world;
	c25_screen_to_world(m_cam, &world, &screen, width, height);

	return Vector(world.x, world.y);
}

Vector Pseudo3DCamera::TransPosProjectToScreen(const Vector& proj, int width, int height) const
{
	return TransPosProjectToScreen(vec3(proj.x, proj.y, 0), width, height);
}

Vector Pseudo3DCamera::TransPosProjectToScreen(const vec3& proj, int width, int height) const
{
	sm_vec3 world;
	world.x = proj.x;
	world.y = proj.y;
	world.z = proj.z;

	sm_ivec2 screen;
	c25_world_to_screen(m_cam, &screen, &world, width, height);

	return Vector(screen.x, screen.y);
}

void Pseudo3DCamera::UpdateModelView() const
{
	const sm_vec3* p = c25_cam_get_pos(m_cam);
	float angle = c25_cam_get_angle(m_cam);
	ShaderMgr::Instance()->SetModelView(vec3(p->x, p->y, p->z), angle);
}

float Pseudo3DCamera::GetScale() const
{
	return 1;
}

const Vector& Pseudo3DCamera::GetPosition() const
{
	return Vector(0, 0);
}

void Pseudo3DCamera::Translate(const vec3& offset)
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

float Pseudo3DCamera::GetZ() const 
{ 
	return c25_cam_get_pos(m_cam)->z;
}

}
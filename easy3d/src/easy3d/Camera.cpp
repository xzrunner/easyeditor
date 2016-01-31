#include "Camera.h"



namespace e3d
{

 const float Camera::CAM_NEAR = 1;
 const float Camera::CAM_FAR = 3;
 static const float DEFAULT_Z = 2;

// const float Camera::CAM_NEAR = 4;
// const float Camera::CAM_FAR = 10;
// static const float DEFAULT_Z = -6;

// const float Camera::CAM_NEAR = 1004;
// const float Camera::CAM_FAR = 1010;
// static const float DEFAULT_Z = -1008;

static const float ZOOM_STEP = 0.05f;

Camera::Camera()
	: m_pos(0, 0, DEFAULT_Z)
	, m_rot_heading(0)
	, m_rot_elevation(-90)
{
}

void Camera::Translate(const vec3& offset)
{
	m_pos += offset;
}

void Camera::Zoom(bool zoomin)
{
	if (zoomin) {
		m_pos.z *= (1 - ZOOM_STEP);
	} else {
		m_pos.z *= (1 + ZOOM_STEP);
	}
}

void Camera::Rotate(float dheading, float delevation)
{
	m_rot_heading += dheading;
	m_rot_elevation += delevation;
}

vec3 Camera::GetLeft() const
{
	// Step 1: n = <target position - view reference point>
	vec3 n = GetToward();
	// Step 2: Let v = <0,1,0>
	vec3 v(0, 1, 0);
	// Step 3: u = (v x n)
	vec3 u = v.Cross(n);

	return u;
}

vec3 Camera::GetUp() const
{
	// Step 1: n = <target position - view reference point>
	vec3 n = GetToward();
	// Step 2: Let v = <0,1,0>
	vec3 v(0, 1, 0);
	// Step 3: u = (v x n)
	vec3 u = v.Cross(n);
	// Step 4: v = (n x u)
	v = n.Cross(u);
	
	return v;
}

vec3 Camera::GetToward() const
{
	// reset rot matrix
	// compute trig functions once
	float rad_elevation = m_rot_elevation * ee::TRANS_DEG_TO_RAD;
	float sin_elevation = sin(rad_elevation);
	float cos_elevation = cos(rad_elevation);

	float rad_heading = m_rot_heading * ee::TRANS_DEG_TO_RAD;
	float sin_heading = sin(rad_heading);
	float cos_heading = cos(rad_heading);

	// now compute the target point on a unit sphere x,y,z
	vec3 target;
	target.x = -1*sin_elevation*sin_heading;
	target.y =  1*cos_elevation;
	target.z =  1*sin_elevation*cos_heading;

	return target;
}

mat4 Camera::GetModelViewMat() const
{
	mat4 trans = mat4::Translate(-m_pos.x, -m_pos.y, -m_pos.z);
	mat4 rot = GetModelViewRotMat();
	return trans * rot;
}

void Camera::SetScreenSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

vec3 Camera::MapToSphere(ivec2 touchpoint) const
{
// 	int viewport[4];
// 	glGetIntegerv(GL_VIEWPORT, viewport);
// 	int width = viewport[2];
// 	int height = viewport[3];

	const float trackball_radius = m_width / 3;
	const vec2 center_point(m_width*0.5f, m_height*0.5f);

	vec2 p = (vec2)touchpoint - center_point;

	// Flip the Y axis because pixel coords increase towards the bottom.
	p.y = -p.y;

	const float radius = trackball_radius;
	const float safeRadius = radius - 1;

	if (p.Length() > safeRadius) {
		float theta = atan2(p.y, p.x);
		p.x = safeRadius * cos(theta);
		p.y = safeRadius * sin(theta);
	}

	float z = sqrt(radius * radius - p.LengthSquared());
	vec3 mapped = vec3(p.x, p.y, z);
	return mapped / radius;
}

void Camera::Reset()
{
	m_pos.x = m_pos.y = 0;
	m_pos.z = DEFAULT_Z;

	m_rot_heading = 0;
	m_rot_elevation = 90;
}

mat4 Camera::GetModelViewRotMat() const
{
	// Step 1: n = <target position - view reference point>
	vec3 n = -GetToward();
	// Step 2: Let v = <0,1,0>
	vec3 v(0, 1, 0);
	// Step 3: u = (v x n)
	vec3 u = v.Cross(n);
	// Step 4: v = (n x u)
	v = n.Cross(u);
	// Step 5: normalize all vectors
	u.Normalize();
	v.Normalize();
	n.Normalize();

	float mt[16] = {
		u.x,    v.x,     n.x,     0,
		u.y,    v.y,     n.y,     0,
		u.z,    v.z,     n.z,     0,
		0,        0,       0,     1
	};
	return mat4(mt);
}

}
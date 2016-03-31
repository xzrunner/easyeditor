#include "Pseudo3DCamera.h"
#include "ShaderMgr.h"
#include "panel_msg.h"

#include <sm.h>

#include <wx/log.h>

namespace ee
{

static const float Z = -1000;
//static const float ANGLE = 0;
static const float ANGLE = -20;

Pseudo3DCamera::Pseudo3DCamera()
	: m_position(0, 0, Z)
	, m_angle(ANGLE)
{
	UpdateModelView();
}

void Pseudo3DCamera::Reset()
{
	m_position.x = m_position.y = 0;
	m_position.z = Z;
	m_angle = ANGLE;
	UpdateModelView();
}

Vector Pseudo3DCamera::TransPosScreenToProject(int x, int y, int width, int height) const
{
// 	Vector proj;
// 	proj.x = 2.0f * x / width - 1;
// 	proj.y = 2.0f * (height - y) / height - 1;
// 	proj.x = - proj.x * m_position.z - m_position.x;
// 	proj.y = - proj.y * m_position.z * height / width - m_position.y;
// 	return proj;

	//////////////////////////////////////////////////////////////////////////

// 	float top_z = m_position.z,
// 		  bottom_z = m_position.z;
// 
// 	float rad = -m_angle * SM_DEG_TO_RAD;
// 	float tan_rad = tanf(rad);
// 	float a = m_position.z / (1 / tan_rad + 1);
// 	float b = m_position.z / (1 / tan_rad - 1);
// 	
// 	top_z += b;
// 	bottom_z -= a;
// 
// //	float z = bottom_z + (top_z - bottom_z) * (height - y) / y;
// 
// 	float y = (height - y) / height;
// 
// 	fabs(y - 0.5f) * m_position.z
// 
// // 	if (y <= 0.5f) {
// // 		a / sinf(rad);
// // 	} else {
// // 
// // 	}
// 
// 	Vector proj;
// 	proj.x = 2.0f * x / width - 1;
// 	proj.y = 2.0f * (height - y) / height - 1;
// 
// 	proj.x = - proj.x * z - m_position.x;
// 	proj.y = - proj.y * z * height / width - m_position.y;
// 	return proj;

	//////////////////////////////////////////////////////////////////////////

	return Vector(0, 0);
}

Vector Pseudo3DCamera::TransPosProjectToScreen(const Vector& proj, int width, int height) const
{
	sm_mat4 mat = GetMatrix(width, height);

	sm_vec3 vec3;
	vec3.x = proj.x;
	vec3.y = proj.y;
	vec3.z = 0;
	sm_vec3_mul(&vec3, &mat);

	wxLogDebug("to screen: %f, %f, %f", vec3.x, vec3.y, vec3.z);

	return Vector(vec3.x, vec3.y);
}

Vector Pseudo3DCamera::TransPosProjectToScreen(const vec3& proj, int width, int height) const
{
	sm_mat4 mat = GetMatrix(width, height);

	sm_vec3 vec3;
	vec3.x = proj.x;
	vec3.y = proj.y;
	vec3.z = proj.z;
	sm_vec3_mul(&vec3, &mat);

	wxLogDebug("to screen: %f, %f, %f", vec3.x, vec3.y, vec3.z);

	return Vector(vec3.x, vec3.y);
}

void Pseudo3DCamera::UpdateModelView() const
{
	ShaderMgr::Instance()->SetModelView(m_position, m_angle);
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
	m_position += offset;
	UpdateModelView();
}

void Pseudo3DCamera::Rotate(float da)
{
	m_angle += da;
	UpdateModelView();
	SetCanvasDirtySJ::Instance()->SetDirty();
}

sm_mat4 Pseudo3DCamera::GetMatrix(int width, int height) const
{
	union sm_mat4 rmat;
	sm_mat4_rotxmat(&rmat, m_angle);

	union sm_mat4 tmat;
	sm_mat4_identity(&tmat);
	sm_mat4_trans(&tmat, m_position.x, m_position.y, m_position.z);

	union sm_mat4 mv_mat;
	sm_mat4_mul(&mv_mat, &rmat, &tmat);

	union sm_mat4 proj_mat;
	float hw = width * 0.5f;
	float hh = height * 0.5f;
	sm_mat4_perspective(&proj_mat, -hw, hw, -hh, hh, NEAR_CLIP, FAR_CLIP);

	union sm_mat4 mat;
	sm_mat4_mul(&mat, &proj_mat, &mv_mat);

	return mat;
}

}
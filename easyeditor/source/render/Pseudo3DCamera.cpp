#include "Pseudo3DCamera.h"
#include "ShaderMgr.h"
#include "panel_msg.h"

namespace ee
{

Pseudo3DCamera::Pseudo3DCamera()
	: m_position(0, 0, -110)
	, m_angle(-2)
{
	UpdateModelView();
}

void Pseudo3DCamera::Reset()
{
}

Vector Pseudo3DCamera::TransPosScreenToProject(int x, int y, int width, int height) const
{
	return Vector(0, 0);
}

Vector Pseudo3DCamera::TransPosProjectToScreen(const Vector& proj, int width, int height) const
{
	return Vector(0, 0);
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

}
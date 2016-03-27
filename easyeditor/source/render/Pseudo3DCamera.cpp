#include "Pseudo3DCamera.h"

namespace ee
{

Pseudo3DCamera::Pseudo3DCamera()
	: m_position(0, 0, -100)
	, m_angle(-5)
{
}

void Pseudo3DCamera::Reset()
{
}

Vector Pseudo3DCamera::TransPosScreenToProject(int x, int y, int width, int height) const
{
	return Vector();
}

Vector Pseudo3DCamera::TransPosProjectToScreen(const Vector& proj, int width, int height) const
{
	return Vector();
}

void Pseudo3DCamera::UpdateModelView() const
{
}

float Pseudo3DCamera::GetScale() const
{
	return 1;
}

const Vector& Pseudo3DCamera::GetPosition() const
{
	return Vector();
}

}
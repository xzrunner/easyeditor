#include "PerspectCamera.h"

namespace ee
{

PerspectCamera::PerspectCamera(float scale)
{

}

void PerspectCamera::Reset()
{

}

Vector PerspectCamera::TransPosScreenToProject(int x, int y, int width, int height) const
{
	return Vector();
}

Vector PerspectCamera::TransPosProjectToScreen(const Vector& proj, int width, int height) const
{
	return Vector();
}

void PerspectCamera::UpdateModelView() const
{
}

float PerspectCamera::GetScale() const
{
	return 1;
}

const Vector& PerspectCamera::GetPosition() const
{
	return Vector();
}

}
#include "PerspectCamera.h"

namespace ee
{

PerspectiveCamera::PerspectiveCamera(float scale)
{

}

void PerspectiveCamera::Reset()
{

}

Vector PerspectiveCamera::TransPosScreenToProject(int x, int y, int width, int height) const
{
	return Vector();
}

Vector PerspectiveCamera::TransPosProjectToScreen(const Vector& proj, int width, int height) const
{
	return Vector();
}

void PerspectiveCamera::UpdateModelView() const
{
}

float PerspectiveCamera::GetScale() const
{
	return 1;
}

const Vector& PerspectiveCamera::GetPosition() const
{
	return Vector();
}

}
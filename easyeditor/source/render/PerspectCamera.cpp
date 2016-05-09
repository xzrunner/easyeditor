#include "PerspectCamera.h"

namespace ee
{

PerspectiveCamera::PerspectiveCamera(float scale)
{

}

void PerspectiveCamera::Reset()
{

}

sm::vec2 PerspectiveCamera::TransPosScreenToProject(int x, int y, int width, int height) const
{
	return sm::vec2();
}

sm::vec2 PerspectiveCamera::TransPosProjectToScreen(const sm::vec2& proj, int width, int height) const
{
	return sm::vec2();
}

void PerspectiveCamera::UpdateModelView() const
{
}

float PerspectiveCamera::GetScale() const
{
	return 1;
}

const sm::vec2& PerspectiveCamera::GetPosition() const
{
	return sm::vec2();
}

}
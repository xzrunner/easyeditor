#include "Screen.h"
#include "Camera.h"

namespace d2d
{

float Screen::GetScale() const
{
	return m_cam->GetScale();
}

void Screen::UpdateModelView() const
{
	m_cam->UpdateModelView();
}

}
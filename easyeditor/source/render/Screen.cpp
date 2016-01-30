#include "Screen.h"
#include "Camera.h"

namespace ee
{

float Screen::GetScale() const
{
	return m_cam->GetScale();
}

void Screen::UpdateModelView() const
{
	m_cam->UpdateModelView();
}

Rect Screen::GetRegion() const
{
	Rect r(m_size.x * m_cam->GetScale(), m_size.y * m_cam->GetScale());
	r.Translate(m_cam->GetPosition());
	return r;
}

}
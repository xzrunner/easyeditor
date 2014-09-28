#include "Screen.h"
#include "Camera.h"

namespace d2d
{

void Screen::UpdateModelView() const
{
	m_cam->UpdateModelView();
}

}
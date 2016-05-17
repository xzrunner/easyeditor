#include "Screen.h"
#include "Camera.h"
#include "CameraMgr.h"

namespace ee
{

float Screen::GetScale() const
{
	return CameraMgr::Instance()->GetCamera()->GetScale();
}

void Screen::UpdateModelView() const
{
	CameraMgr::Instance()->GetCamera()->UpdateModelView();
}

sm::rect Screen::GetRegion() const
{
	Camera* cam = CameraMgr::Instance()->GetCamera();
	sm::rect r(sm::vec2(0, 0), m_size.x * cam->GetScale(), m_size.y * cam->GetScale());
	r.Translate(cam->GetPosition());
	return r;
}

}
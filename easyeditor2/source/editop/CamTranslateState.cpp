#include "ee2/CamTranslateState.h"

#include <ee0/SubjectMgr.h>

#include <sprite2/OrthoCamera.h>
#include <sprite2/Pseudo3DCamera.h>
#include <sprite2/RenderCtxStack.h>

namespace ee2
{

CamTranslateState::CamTranslateState(s2::Camera& cam, ee0::SubjectMgr& sub_mgr)
	: m_cam(cam)
	, m_sub_mgr(sub_mgr)
{
	m_last_pos.MakeInvalid();
}

bool CamTranslateState::OnMousePress(int x, int y)
{
	m_last_pos.Set(x, y);
	return false;
}

bool CamTranslateState::OnMouseRelease(int x, int y)
{
	m_last_pos.MakeInvalid();
	return false;
}

bool CamTranslateState::OnMouseDrag(int x, int y)
{
	if (!m_last_pos.IsValid()) {
		return false;
	}

	auto offx = m_last_pos.x - x;
	auto offy = y - m_last_pos.y;
	switch (m_cam.Type())
	{
	case s2::CAM_ORTHO2D:
		{
			auto& cam = dynamic_cast<s2::OrthoCamera&>(m_cam);
			cam.Translate(sm::vec2(
				static_cast<float>(offx), 
				static_cast<float>(offy)));
		}
		break;
	case s2::CAM_PSEUDO3D:
		{
			auto& cam = dynamic_cast<s2::Pseudo3DCamera&>(m_cam);
			auto ctx = s2::RenderCtxStack::Instance()->Top();
			if (!ctx) {
				return false;
			}
			int w = ctx->GetScreenWidth(),
				h = ctx->GetScreenHeight();
			sm::vec2 last = cam.TransPosScreenToProject(m_last_pos.x, m_last_pos.y, w, h);
			sm::vec2 curr = cam.TransPosScreenToProject(x, y, w, h);
			cam.Translate(sm::vec3(curr.x - last.x, curr.y - last.y, 0));
		}
		break;
	}

	m_last_pos.Set(x, y);

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

	return false;
}

}
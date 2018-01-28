#pragma once

#include <ee0/EditOpState.h>

namespace ee0 { class SubjectMgr; }
namespace s2 { class Camera; }

namespace ee2
{

class CamZoomState : public ee0::EditOpState
{
public:
	CamZoomState(s2::Camera& cam, ee0::SubjectMgr& sub_mgr);

	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
	s2::Camera&      m_cam;
	ee0::SubjectMgr& m_sub_mgr;

}; // CamZoomState

}
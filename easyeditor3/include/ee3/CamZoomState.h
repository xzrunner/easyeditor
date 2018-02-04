#pragma once

#include <ee0/EditOpState.h>

namespace n3 { class Camera; class Viewport; }
namespace ee0 { class SubjectMgr; }

namespace ee3
{

class CamZoomState : public ee0::EditOpState
{
public:
	CamZoomState(n3::Camera& cam, const n3::Viewport& vp,
		ee0::SubjectMgr& sub_mgr);

	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
	n3::Camera&         m_cam;
	const n3::Viewport& m_vp;
	ee0::SubjectMgr&    m_sub_mgr;

}; // CamZoomState

}
#pragma once

#include <ee0/EditOpState.h>

namespace pt3 { class Camera; class Viewport; }
namespace ee0 { class SubjectMgr; }

namespace ee3
{

class CamZoomState : public ee0::EditOpState
{
public:
	CamZoomState(pt3::Camera& cam, const pt3::Viewport& vp,
		ee0::SubjectMgr& sub_mgr);

	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
	pt3::Camera&         m_cam;
	const pt3::Viewport& m_vp;
	ee0::SubjectMgr&    m_sub_mgr;

}; // CamZoomState

}
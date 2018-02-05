#pragma once

#include <ee0/EditOpState.h>

#include <SM_Vector.h>

namespace pt3 { class Camera; }
namespace ee0 { class SubjectMgr; }

namespace ee3
{

class CamRotateState : public ee0::EditOpState
{
public:
	CamRotateState(pt3::Camera& cam, ee0::SubjectMgr& sub_mgr);

	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseRelease(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

private:
	pt3::Camera&      m_cam;
	ee0::SubjectMgr& m_sub_mgr;

	sm::ivec2 m_last_pos;

}; // CamRotateState

}
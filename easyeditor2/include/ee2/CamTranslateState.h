#pragma once

#include <ee0/EditOpState.h>

#include <SM_Vector.h>

namespace ee0 { class SubjectMgr; }
namespace s2 { class Camera; }

namespace ee2
{

class CamTranslateState : public ee0::EditOpState
{
public:
	CamTranslateState(s2::Camera& cam, ee0::SubjectMgr& sub_mgr);

	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseRelease(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

private:
	s2::Camera&      m_cam;
	ee0::SubjectMgr& m_sub_mgr;

	sm::ivec2 m_last_pos;

}; // CamTranslateState

}

#pragma once

#include <ee0/EditOpState.h>

#include <SM_Vector.h>

namespace s2 { class Camera; }
namespace ee0 { class SubjectMgr; }

namespace ee2
{

class DrawSelectRectState : public ee0::EditOpState
{
public:
	DrawSelectRectState(s2::Camera& cam, ee0::SubjectMgr& sub_mgr);
	
	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseRelease(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

private:
	s2::Camera&      m_cam;
	ee0::SubjectMgr& m_sub_mgr;

	sm::vec2 m_first_pos, m_last_pos;

}; // DrawSelectRectState

}
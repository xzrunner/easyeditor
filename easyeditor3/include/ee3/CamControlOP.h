#pragma once

#include <ee0/EditOP.h>
#include <ee0/typedef.h>

namespace pt3 { class Camera; class Viewport; }
namespace ee0 { class SubjectMgr; }

namespace ee3
{

class Camera;

class CamControlOP : public ee0::EditOP
{
public:
	CamControlOP(pt3::Camera& cam, const pt3::Viewport& vp, ee0::SubjectMgr& sub_mgr);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
	void ChangeEditOpState(const ee0::EditOpStatePtr& state);

private:
	pt3::Camera&         m_cam;
	ee0::SubjectMgr&    m_sub_mgr;

	ee0::EditOpStatePtr m_op_state = nullptr;

	ee0::EditOpStatePtr m_rotate_state = nullptr;
	ee0::EditOpStatePtr m_translate_state = nullptr;
	ee0::EditOpStatePtr m_zoom_state = nullptr;

}; // CamControlOP

}
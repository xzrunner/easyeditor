#pragma once

#include <ee0/EditOpState.h>

#include <ee/EditOP.h>

#include <memory>

namespace n3 { class Camera; class Viewport; }
namespace ee0 { class SubjectMgr; }

namespace ee3
{

class Camera;

class CamControlOP : public ee::EditOP
{
public:
	CamControlOP(wxWindow* wnd, ee::EditPanelImpl* stage,
		n3::Camera& cam, const n3::Viewport& vp, ee0::SubjectMgr& sub_mgr);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
	n3::Camera&         m_cam;
	const n3::Viewport& m_vp;
	ee0::SubjectMgr&    m_sub_mgr;

	std::unique_ptr<ee0::EditOpState> m_op_state = nullptr;

}; // CamControlOP

}
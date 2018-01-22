#pragma once

#include "EditOpState.h"

#include <ee/EditOP.h>

#include <memory>

namespace n3 { class Camera; class Viewport; }

namespace enode3d
{

class Camera;

class CamControlOP : public ee::EditOP
{
public:
	CamControlOP(wxWindow* wnd, ee::EditPanelImpl* stage,
		n3::Camera& cam, const n3::Viewport& vp);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
	n3::Camera& m_cam;
	const n3::Viewport& m_vp;

	std::unique_ptr<EditOpState> m_op_state = nullptr;

}; // CamControlOP

}
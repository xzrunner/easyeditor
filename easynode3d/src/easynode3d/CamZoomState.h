#pragma once

#include "EditOpState.h"

namespace n3 { class Camera; class Viewport; }

namespace enode3d
{

class CamZoomState : public EditOpState
{
public:
	CamZoomState(n3::Camera& cam, const n3::Viewport& vp);

	virtual void OnMouseWheelRotation(int x, int y, int direction) override;

private:
	n3::Camera& m_cam;
	const n3::Viewport& m_vp;

}; // CamZoomState

}
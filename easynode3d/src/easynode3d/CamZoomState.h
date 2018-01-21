#pragma once

#include "EditOpState.h"

namespace ee { class StageCanvas; }
namespace n3 { class Camera; class Viewport; }

namespace enode3d
{

class StageCanvas;

class CamZoomState : public EditOpState
{
public:
	CamZoomState(ee::StageCanvas& canvas, n3::Camera& cam,
		const n3::Viewport& vp);

	virtual void OnMouseWheelRotation(int x, int y, int direction) override;

private:
	ee::StageCanvas& m_canvas;

	n3::Camera& m_cam;
	const n3::Viewport& m_vp;

}; // CamZoomState

}
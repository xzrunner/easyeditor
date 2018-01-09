#pragma once

#include "EditOpState.h"

namespace n3 { class Camera; }

namespace enode3d
{

class StageCanvas;

class CamZoomState : public EditOpState
{
public:
	CamZoomState(StageCanvas& canvas, n3::Camera& cam);

	virtual void OnMouseWheelRotation(int x, int y, int direction) override;

private:
	StageCanvas& m_canvas;

	n3::Camera& m_cam;

}; // CamZoomState

}
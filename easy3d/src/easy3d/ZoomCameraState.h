#pragma once

#include "EditOpState.h"

namespace n3 { class Camera; }

namespace e3d
{

class StageCanvas;

class ZoomCameraState : public EditOpState
{
public:
	ZoomCameraState(StageCanvas& canvas, n3::Camera& cam);

	virtual void OnMouseWheelRotation(int x, int y, int direction) override;

private:
	StageCanvas& m_canvas;

	n3::Camera& m_cam;

}; // ZoomCameraState

}
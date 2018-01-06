#pragma once

#include "EditOpState.h"

namespace e3d
{

class StageCanvas;
class Camera;

class RotateCameraState : public EditOpState
{
public:
	RotateCameraState(StageCanvas& canvas, Camera& cam, 
		const sm::ivec2& pos);

	virtual void OnMousePress(const sm::ivec2& pos);
	virtual void OnMouseRelease(const sm::ivec2& pos);
	virtual void OnMouseDrag(const sm::ivec2& pos);

private:
	StageCanvas& m_canvas;

	Camera& m_cam;

	sm::ivec2 m_last_pos;

}; // RotateCameraState

}
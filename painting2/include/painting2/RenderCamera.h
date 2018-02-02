#pragma once

#include "painting2/CameraMode.h"

namespace pt2
{

class RenderCamera
{
public:
	RenderCamera();

	void Reset();

	bool operator == (const RenderCamera& cam) const;
	bool operator != (const RenderCamera& cam) const;

	RenderCamera operator * (const RenderCamera& rc) const;

	void CalculateZ(float cam_angle, const float vertices[8], float z[4]) const;

	CameraMode GetMode() const { return m_mode; }
	void SetMode(CameraMode mode) { m_mode = mode; }

private:
	CameraMode m_mode;
	float      m_base_y;

}; // RenderCamera

}
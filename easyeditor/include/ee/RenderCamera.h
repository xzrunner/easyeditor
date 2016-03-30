#ifndef _EASYEDITOR_RENDER_CAMERA_H_
#define _EASYEDITOR_RENDER_CAMERA_H_

#include "Vector.h"
#include "CameraModes.h"

namespace ee
{

class Pseudo3DCamera;

class RenderCamera
{
public:
	RenderCamera();
	
	void CalculateZ(const Pseudo3DCamera* cam, Vector vertices[4], float z[4]) const;

public:
	CameraMode mode;

	float base_y;

}; // RenderCamera

}

#endif // _EASYEDITOR_RENDER_CAMERA_H_
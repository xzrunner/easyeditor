#ifndef _EASYEDITOR_RENDER_CAMERA_H_
#define _EASYEDITOR_RENDER_CAMERA_H_

#include "CameraModes.h"

#include <SM_Vector.h>

namespace ee
{

class Pseudo3DCamera;

class RenderCamera
{
public:
	RenderCamera();
	
	void CalculateZ(const Pseudo3DCamera* cam, sm::vec2 vertices[4], float z[4]) const;

public:
	CameraMode mode;

	float base_y;

}; // RenderCamera

}

#endif // _EASYEDITOR_RENDER_CAMERA_H_
#ifndef _SPRITE2_RENDER_CAMERA_H_
#define _SPRITE2_RENDER_CAMERA_H_

#include "CameraMode.h"

namespace s2
{

class RenderCamera
{
public:
	CameraMode	mode;
	float		base_y;	

public:
	RenderCamera()
		: mode(CM_ORTHO)
		, base_y(0)
	{}

}; // RenderCamera

}

#endif // _SPRITE2_RENDER_CAMERA_H_
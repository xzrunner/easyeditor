#ifndef _EASYEDITOR_RENDER_CAMERA_H_
#define _EASYEDITOR_RENDER_CAMERA_H_

#include "Vector.h"

namespace ee
{

class Pseudo3DCamera;

class RenderCamera
{
public:
	RenderCamera();
	
	void CalculateZ(const Pseudo3DCamera* cam, Vector vertices[4], float z[4]) const;

	float CalculateDZ(const Pseudo3DCamera* cam, float offy);

public:
	bool has_height;

	float start_height;

}; // RenderCamera

}

#endif // _EASYEDITOR_RENDER_CAMERA_H_
#ifndef _EASYEDITOR_RENDER_CAMERA_H_
#define _EASYEDITOR_RENDER_CAMERA_H_

#include <sprite2/RenderCamera.h>

#include <SM_Vector.h>

namespace ee
{

class Pseudo3DCamera;

class RenderCamera : public s2::RenderCamera
{
public:
	void CalculateZ(const Pseudo3DCamera* cam, sm::vec2 vertices[4], float z[4]) const;

}; // RenderCamera

}

#endif // _EASYEDITOR_RENDER_CAMERA_H_
#ifndef _GLUE_CAMERA_H_
#define _GLUE_CAMERA_H_

namespace glue
{

class Camera
{
public:
	virtual ~Camera() {}
	virtual void OnSize(int width, int height) = 0;
	virtual void Reset() = 0;

}; // Camera

}

#endif // _GLUE_CAMERA_H_
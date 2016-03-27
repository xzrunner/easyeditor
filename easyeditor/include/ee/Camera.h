#ifndef _EASYEDITOR_CAMERA_H_
#define _EASYEDITOR_CAMERA_H_

#include "Vector.h"

namespace ee
{

class Camera
{
public:
	virtual ~Camera() {}

	virtual std::string Type() const = 0;
	virtual void Reset() = 0;

	virtual Vector TransPosScreenToProject(int x, int y, int width, int height) const = 0;
	virtual Vector TransPosProjectToScreen(const Vector& proj, int width, int height) const = 0;

	virtual void UpdateModelView() const = 0;

	virtual float GetScale() const = 0;
	virtual const Vector& GetPosition() const = 0;

}; // Camera

}

#endif // _EASYEDITOR_CAMERA_H_
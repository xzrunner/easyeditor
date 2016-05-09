#ifndef _EASYEDITOR_CAMERA_H_
#define _EASYEDITOR_CAMERA_H_

#include <SM_Vector.h>

#include <string>

namespace ee
{

class Camera
{
public:
	virtual ~Camera() {}

	virtual std::string Type() const = 0;
	virtual void Reset() = 0;

	virtual sm::vec2 TransPosScreenToProject(int x, int y, int width, int height) const = 0;
	virtual sm::vec2 TransPosProjectToScreen(const sm::vec2& proj, int width, int height) const = 0;

	virtual void UpdateModelView() const = 0;

	virtual float GetScale() const = 0;
	virtual const sm::vec2& GetPosition() const = 0;

}; // Camera

}

#endif // _EASYEDITOR_CAMERA_H_
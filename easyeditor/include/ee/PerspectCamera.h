#ifndef _EASYEDITOR_PERSPECTIVE_CAMERA_H_
#define _EASYEDITOR_PERSPECTIVE_CAMERA_H_

#include "Camera.h"

namespace ee
{

class PerspectiveCamera : public Camera
{
public:
	PerspectiveCamera(float scale = 1.0f);

	virtual std::string Type() const { return "perspective"; }
	virtual void Reset();

	virtual sm::vec2 TransPosScreenToProject(int x, int y, int width, int height) const;
	virtual sm::vec2 TransPosProjectToScreen(const sm::vec2& proj, int width, int height) const;

	virtual void UpdateModelView() const;

	virtual float GetScale() const;
	virtual const sm::vec2& GetPosition() const;

}; // PerspectiveCamera

}

#endif // _EASYEDITOR_PERSPECTIVE_CAMERA_H_
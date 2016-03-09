#ifndef _EASYEDITOR_PERSPECT_CAMERA_H_
#define _EASYEDITOR_PERSPECT_CAMERA_H_

#include "Camera.h"

namespace ee
{

class PerspectCamera : public Camera
{
public:
	PerspectCamera(float scale = 1.0f);

	virtual void Reset();

	virtual Vector TransPosScreenToProject(int x, int y, int width, int height) const;
	virtual Vector TransPosProjectToScreen(const Vector& proj, int width, int height) const;

	virtual void UpdateModelView() const;

	virtual float GetScale() const;
	virtual const Vector& GetPosition() const;

}; // PerspectCamera

}

#endif // _EASYEDITOR_PERSPECT_CAMERA_H_
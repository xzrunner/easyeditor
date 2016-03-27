#ifndef _EASYEDITOR_PSEUDO3D_CAMERA_H_
#define _EASYEDITOR_PSEUDO3D_CAMERA_H_

#include "Camera.h"
#include "Vector3D.h"

namespace ee
{

class Pseudo3DCamera : public Camera
{
public:
	Pseudo3DCamera();

	virtual void Reset();

	virtual Vector TransPosScreenToProject(int x, int y, int width, int height) const;
	virtual Vector TransPosProjectToScreen(const Vector& proj, int width, int height) const;

	virtual void UpdateModelView() const;

	virtual float GetScale() const;
	virtual const Vector& GetPosition() const;

private:
	vec3 m_position;

	float m_angle;

}; // Pseudo3DCamera

}

#endif // _EASYEDITOR_PSEUDO3D_CAMERA_H_
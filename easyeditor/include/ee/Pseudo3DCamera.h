#ifndef _EASYEDITOR_PSEUDO3D_CAMERA_H_
#define _EASYEDITOR_PSEUDO3D_CAMERA_H_

#include "Camera.h"
#include "Vector3D.h"

union sm_mat4;

namespace ee
{

class Pseudo3DCamera : public Camera
{
public:
	Pseudo3DCamera();

	virtual std::string Type() const { return "pseudo3d"; }
	virtual void Reset();

	virtual Vector TransPosScreenToProject(int x, int y, int width, int height) const;
	virtual Vector TransPosProjectToScreen(const Vector& proj, int width, int height) const;

	Vector TransPosProjectToScreen(const vec3& proj, int width, int height) const;

	virtual void UpdateModelView() const;

	virtual float GetScale() const;
	virtual const Vector& GetPosition() const;

	void Translate(const vec3& offset);
	void Rotate(float da);

	float GetAngle() const { return m_angle; }
	float GetZ() const { return m_position.z; }

public:
	static const int FAR_CLIP = 800;
	static const int NEAR_CLIP = 50;

private:
	sm_mat4 GetMatrix(int width, int height) const;

private:
	vec3 m_position;

	float m_angle;

}; // Pseudo3DCamera

}

#endif // _EASYEDITOR_PSEUDO3D_CAMERA_H_